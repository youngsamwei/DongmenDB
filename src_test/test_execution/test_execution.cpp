//
// Created by sam on 2018/11/3.
//

#include <ctime>
#include <io.h>
#include <cstring>
#include "test_execution.h"
#include <afxres.h>

int TestExecution::run(const char *exp_name, const char *exp_target, const char *exp_dir_name, const char *work_dir, const char *dongmendb_src_dir) {

    const char *str = rand_str(10);
    char *current_dir = (char *)malloc((strlen(work_dir) + strlen(project_name) + strlen(str) + 2)*sizeof(char));
    strcpy(current_dir, work_dir);
    strcat(current_dir, "/");
    strcat(current_dir, project_name);
    strcat(current_dir, "_");
    strcat(current_dir, str);

    int slen = strlen(current_dir) + strlen(cmake_build_dir) + 1;
    char *build_dir = (char *)malloc(slen*sizeof(char));
    strcpy(build_dir, current_dir);
    strcat(build_dir, "/");
    strcat(build_dir, cmake_build_dir);

    slen = strlen(current_dir) + 4;
    char *bin_dir = (char *)malloc(slen*sizeof(char));
    strcpy(bin_dir, current_dir);
    strcat(bin_dir, "/bin");

    int c  = init_dongmendb(work_dir, current_dir);

    copy_dongmendb(dongmendb_src_dir, current_dir);

    /*复制作业到工作目录*/
    copy_exp_to_dongmendb(exp_dir_name, current_dir);

    cmd_cmake_refresh(build_dir, current_dir);

    cmd_cmake_clean(build_dir);

    cmd_cmake_build(build_dir, exp_target);

    cmd_exp_target(bin_dir, exp_target);

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

int TestExecution::cmd_cmake_refresh(const char*output_dir, const char *project_dir){

    int slen = strlen(cmake_exe) + strlen(cmake_build_type) + strlen(cmake_files) + strlen(project_dir);
    char *cmd = (char *)malloc(sizeof(char) * slen);
    strcpy(cmd, cmake_exe);
    strcat(cmd, cmake_build_type);
    strcat(cmd, cmake_files);
    strcat(cmd, project_dir);

    removeDir(output_dir);
    mkdir(output_dir);
    chdir(output_dir);
    char result[1024] = {0};
    executeCMD(cmd, result);
    printf("\n");
    printf(cmd);
    printf(result);
    printf("\n");
    return 0;
}

int TestExecution::cmd_cmake_clean(const char *build_dir_name) {

    int slen = strlen(cmake_exe) + strlen(cmd_build) + strlen(build_dir_name) + strlen(cmd_target_clean);
    char *cmd = (char *)malloc(sizeof(char) * slen);
    strcpy(cmd, cmake_exe);
    strcat(cmd, cmd_build);
    strcat(cmd, build_dir_name);
    strcat(cmd, cmd_target_clean);

    chdir(build_dir_name);

    char result[1024] = {0};
    printf("\n");
    printf(cmd);
    executeCMD(cmd, result);
    printf(result);
    printf("\n");
    return 0;
}

int TestExecution::cmd_cmake_build(const char *build_dir_name, const char *exp_target) {

    int slen = strlen(cmake_exe) + strlen(cmd_build)
               + strlen(build_dir_name) + strlen(cmd_target)
               + strlen(exp_target) + strlen(cmake_others_parameters);
    char *cmd = (char *)malloc(sizeof(char) * slen);
    strcpy(cmd, cmake_exe);
    strcat(cmd, cmd_build);
    strcat(cmd, build_dir_name);
    strcat(cmd, cmd_target);
    strcat(cmd, exp_target);
    strcat(cmd, cmake_others_parameters);

    chdir(build_dir_name);

    char result[1024] = {0};
    printf("\n");
    printf(cmd);
    executeCMD(cmd, result);
    printf(result);
    printf("\n");
    return 0;
}

int TestExecution::cmd_exp_target(const char *bin_dir, const char *exp_target) {

    chdir(bin_dir);

    char result[1024] = {0};
    printf("\n");
    printf(exp_target);
    executeCMD(exp_target, result);
    printf(result);
    printf("\n");
    return 0;
}

int TestExecution::cmd_get_test_result() {
    return 0;
}

char *TestExecution::rand_str(size_t len) {
    srand((unsigned)time(NULL));
    char *ch = (char *)malloc((len + 1)*sizeof(char*));
    memset(ch, 0, len + 1);
    for (int i = 0; i < len; ++i)
    {
        int x = rand();
        int l = (strlen(CCH) - 1);
        int p = RAND_MAX / l ;
        x = x / p;

        ch[i] = CCH[x];
    }
    ch[len + 1] = '\0';
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
                    mkdir(dest_path);
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
    char buf_ps[1024]={0};
    FILE *ptr;
    ptr = popen(cmd, "rt");
    if(ptr!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
            if(strlen(result)>1024)
                continue;
            else
                strcat(result, buf_ps);
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", cmd);
    }
    return 0;
}

int TestExecution::executeCMD(){
    char   psBuffer[128];
    FILE   *pPipe;

    /* Run DIR so that it writes its output to a pipe. Open this
     * pipe with read text attribute so that we can read it
     * like a text file.
     */

    if( (pPipe = _popen( "dir ", "rt" )) == NULL )
        exit( 1 );

    /* Read pipe until end of file, or an error occurs. */

    while(fgets(psBuffer, 128, pPipe))
    {
        printf(psBuffer);
    }

    /* Close pipe and print return value of pPipe. */
    if (feof( pPipe))
    {
        printf( "\nProcess returned %d\n", _pclose( pPipe ) );
    }
    else
    {
        printf( "Error: Failed to read the pipe to the end.\n");
    }

}