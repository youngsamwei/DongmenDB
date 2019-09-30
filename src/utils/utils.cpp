//
// Created by Sam on 2018/1/25.
//

#include <stddef.h>
#include <ctype.h>
#include <malloc.h>
#include <dongmendb/DongmenDB.h>
#include <dongmensql/expression.h>
#include <io.h>
#include <direct.h>
#include "utils/utils.h"

char *new_id_name(){
    char *name = (char *) calloc(MAX_ID_NAME_LENGTH, sizeof(char));
//    char *name = (char *)malloc(MAX_ID_NAME_LENGTH);
//    memset(name, 0, MAX_ID_NAME_LENGTH);
    return name;
};

/*
    * BKDR string hash function. Based on the works of Kernighan, Dennis and Pike.
    *
    * Copyleft(or right) 2011         fairywell
    */
unsigned int bkdr_hash(const char *str)
{
    unsigned int seed = 131; // the magic number, 31, 131, 1313, 13131, etc.. orz..
    unsigned int hash = 0;

    unsigned char *p = (unsigned char *) str;
    while (*p){
        hash = hash*seed + (*p++);
    }
    //0x7FFFFFFF是long int 最大值
    return  hash&0x7FFFFFFF;
}


int bytes2int(unsigned char x1, unsigned char x2, unsigned char x3, unsigned char x4){
    return (x1 << 24) | (x2 << 16) | (x3 << 8) | x4;
};

int dongmendb_tokenize(char *str, char ***tokens)
{
    char *s;
    int ntokens = 0;

    s = str;
    if (s==NULL)
        return DONGMENDB_ENOMEM;

    /* First pass: Add \0 at the end of each token
     * and count the number of tokens */
    while(isspace(*s)) s++;

    while(*s != '\0')
    {
        ntokens++;
        if (*s == '"')
        {
            s++;
            while(*s && *s != '"') s++;
        }
        else
            while(*s && !isspace(*s)) s++;

        if(*s != '\0')
        {
            *s++ = '\0';
            while(*s && isspace(*s)) s++;
        }
    }


    /* Second pass: Create the array of tokens */
    *tokens = (char**)malloc(sizeof(char**) * ntokens);

    s = str;
    while(isspace(*s)) s++;
    for(int i=0; i<ntokens; i++)
    {
        if (*s == '"')
        {
            s++;
            (*tokens)[i] = s;
            while(*s && *s != '"') s++;
        }
        else
        {
            (*tokens)[i] = s;
            while(*s && !isspace(*s)) s++;
        }

        s++;
        while(*s && isspace(*s)) s++;
    }

    return ntokens;
}

/*
函数入口：文件夹的绝对路径
          const char*  dirPath
函数功能：删除该文件夹，包括其中所有的文件和文件夹
返回值：  0  删除
         -1  路径不对，或其它情况，没有执行删除操作
*/
int  removeDir(const char*  dirPath)
{

    struct _finddata_t fb;   //查找相同属性文件的存储结构体
    char  path[250];
    long    handle;
    int  resultone;
    int   noFile;            //对系统隐藏文件的处理标记

    noFile = 0;
    handle = 0;


    //制作路径
    strcpy(path,dirPath);
    strcat (path,"/*");

    handle = _findfirst(path,&fb);
    //找到第一个匹配的文件
    if (handle != 0)
    {
        //当可以继续找到匹配的文件，继续执行
        while (0 == _findnext(handle,&fb))
        {
            //windows下，常有个系统文件，名为“..”,对它不做处理
            noFile = strcmp(fb.name,"..");

            if (0 != noFile)
            {
                //制作完整路径
                memset(path,0,sizeof(path));
                strcpy(path,dirPath);
                strcat(path,"/");
                strcat (path,fb.name);
                //属性值为16，则说明是文件夹，迭代
                if (fb.attrib == 16)
                {
                    removeDir(path);
                }
                    //非文件夹的文件，直接删除。对文件属性值的情况没做详细调查，可能还有其他情况。
                else
                {
                    remove(path);
                }
            }
        }
        //关闭文件夹，只有关闭了才能删除。找这个函数找了很久，标准c中用的是closedir
        //经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。
        _findclose(handle);
    }
    //移除文件夹
    resultone = rmdir(dirPath);
    return  resultone;
}


/*
函数入口：文件夹的绝对路径
          const wchar_t*  dirPath
函数功能：删除该文件夹，包括其中所有的文件和文件夹
返回值：  0  删除
         -1  路径不对，或其它情况，没有执行删除操作
*/
int  removeDirW(const wchar_t*  dirPath)
{

    struct _wfinddata_t fb;   //查找相同属性文件的存储结构体

    wchar_t  path[250];
    long    handle;
    int  resultone;
    int   noFile;            //对系统隐藏文件的处理标记

    noFile = 0;
    handle = 0;

    //制作路径
    wcscpy(path,dirPath);
    wcscat (path,L"/*");

    handle = _wfindfirst(path,&fb);
    //找到第一个匹配的文件
    if (handle != 0)
    {
        //当可以继续找到匹配的文件，继续执行
        while (0 == _wfindnext(handle,&fb))
        {
            //windows下，常有个系统文件，名为“..”,对它不做处理
            noFile = wcscmp(fb.name, L"..");

            if (0 != noFile)
            {
                //制作完整路径
                memset(path,0,sizeof(path));
                wcscpy(path,dirPath);
                wcscat(path,L"/");
                wcscat (path,fb.name);
                //属性值为16，则说明是文件夹，迭代
                if (fb.attrib == 16)
                {
                    removeDirW(path);
                }
                    //非文件夹的文件，直接删除。对文件属性值的情况没做详细调查，可能还有其他情况。
                else
                {
                    _wremove(path);
                }
            }
        }
        //关闭文件夹，只有关闭了才能删除。找这个函数找了很久，标准c中用的是closedir
        //经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。
        _findclose(handle);
    }
    //移除文件夹
    resultone = _wrmdir(dirPath);
    return  resultone;
}

