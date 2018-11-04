//
// Created by sam on 2018/11/3.
//

#include <ctime>
#include <io.h>
#include <cstring>
#include "test_execution.h"
#include <afxres.h>

int TestExecution::run(const char *exp_name, const char *exp_target, const char *exp_dir_name, const char *work_dir, const char *dongmendb_src_dir) {

    const char *str = rand_str();
    current_dir = strdup(work_dir);
    strcat(current_dir, "_");
    strcat(current_dir, str);

    int c  = init_dongmendb(work_dir, current_dir);

    copy_dongmendb(dongmendb_src_dir, current_dir);

    return 0;
}

int TestExecution::init_dongmendb(const char *work_dir, const char *dir_name) {
    chdir(work_dir);
    mkdir(dir_name);

    int exists = access(dir_name, F_OK);
    if (exists != 0) {
        return -1;
    }
    return 0;
}

int TestExecution::copy_dongmendb(const char *from_dir_name, const char *dest_dir_name){

    return copyDir(from_dir_name, dest_dir_name);
};

int TestExecution::copy_exp_to_dongmendb(const char *from_dir_name, const char *dest_dir_name) {
    return 0;
}

int TestExecution::cmd_cmake_clean(const char *build_dir_name) {

    chdir(build_dir_name);
    char cmd[250] = {0};
    memset(cmd,0, sizeof(cmake_exe));
    strcpy(cmd, cmake_exe);
    strcat(cmd, " --build ");

    strcat(cmd, build_dir_name);
    strcat(cmd, " --target clean -- -j 2");

    char result[1024] = {0};
    return executeCMD(cmd, result);
}

int TestExecution::cmd_cmake_build(const char *build_dir_name, const char *exp_target) {

    chdir(build_dir_name);
    char cmd[250];
    memset(cmd,0, sizeof(cmake_exe));
    strcpy(cmd, cmake_exe);
    strcat(cmd, " --build ");

    strcat(cmd, build_dir_name);
    strcat(cmd, " --target --");
    strcat(cmd, exp_target);
            strcat(cmd, " -- -j 2");

    char result[1024] = {0};
    return executeCMD(cmd, result);
}

int TestExecution::cmd_exp_target(const char *build_dir_name, const char *exp_target) {
    chdir(build_dir_name);
    char cmd[250]="../bin/";

    strcat(cmd, exp_target);

    char result[1024] = {0};
    return executeCMD(cmd, result);
}

int TestExecution::cmd_get_test_result() {
    return 0;
}

char *TestExecution::rand_str(size_t len) {
    srand((unsigned)time(NULL));
    char *ch;
    ch = (char *)malloc(len);
    memset(ch, 0, len);
    for (int i = 0; i < len; ++i)
    {
        int x = rand() / (RAND_MAX / (sizeof(CCH) - 1));

        ch[i] = CCH[x];
    }
    return ch;
}

char *TestExecution::rand_str() {
    return rand_str(SIZE_RAND_STR_LEN);
}


int TestExecution::copyDir(const char *src_dir, const char *dest_dir){
    struct _finddata_t fb;   //查找相同属性文件的存储结构体
    char  path[250];
    char dest_path[250];
    long    handle;
    int  resultone = 0;
    int   noFile;            //对系统隐藏文件的处理标记

    noFile = 0;
    handle = 0;

    strcpy(dest_path, dest_dir);

    //制作路径
    strcpy(path,src_dir);
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
                memset(dest_path, 0, sizeof(dest_path));
                strcpy(dest_path,dest_dir);
                strcat(dest_path,"/");
                strcat (dest_path,fb.name);

                //制作完整路径
                memset(path,0,sizeof(path));
                strcpy(path,src_dir);
                strcat(path,"/");
                strcat (path,fb.name);
                //属性值为16，则说明是文件夹，迭代
                if (fb.attrib == 16)
                {
                    copyDir(path, dest_path);
                }  //非文件夹的文件，直接复制。对文件属性值的情况没做详细调查，可能还有其他情况。
                else
                {
                    CopyFile(path, dest_path, TRUE);
                }
            }
        }
        //关闭文件夹。找这个函数找了很久，标准c中用的是closedir
        //经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。
        _findclose(handle);
    }
    return  resultone;
}

int TestExecution::executeCMD(const char *cmd, char *result)
{
    char buf_ps[1024];
    char ps[1024]={0};
    FILE *ptr;
    strcpy(ps, cmd);
    if((ptr=popen(ps, "r"))!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
            strcat(result, buf_ps);
            if(strlen(result)>1024)
                break;
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", ps);
    }
}