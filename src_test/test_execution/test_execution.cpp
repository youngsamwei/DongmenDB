//
// Created by sam on 2018/11/3.
//

#include <ctime>
#include <io.h>
#include <cstring>
#include "test_execution.h"
#include <afxres.h>
#include <iostream>

int TestExecution::run(string exp_name, string exp_target, string exp_dir_name,
                       std::map<string, string> exp_files,
                       string work_dir, string dongmendb_src_dir) {

    string str = rand_str(10);
    string current_dir = work_dir + "/" + project_name + "_" + str;
    string build_dir = current_dir + "/" + cmake_build_dir;
    string bin_dir = current_dir + "/bin";

    int c  = init_dongmendb(work_dir, current_dir);

    copy_dongmendb(dongmendb_src_dir, current_dir);

    /*复制作业到工作目录*/
    int ret = copy_exp_to_dongmendb(exp_dir_name, current_dir, exp_files);

    /*缺少实验任务文件*/
    if (ret < 0){
        return -1;
    }

    cmd_cmake_refresh(build_dir, current_dir);

    cmd_cmake_clean(build_dir);

    cmd_cmake_build(build_dir, exp_target);

    cmd_exp_target(bin_dir, exp_target);

    return 0;
}

int TestExecution::init_dongmendb(string work_dir, string dir_name) {
    chdir(work_dir.c_str());
    mkdir(dir_name.c_str());

    int exists = access(dir_name.c_str(), F_OK);
    if (exists != 0) {
        return -1;
    }
    return 0;
}

int TestExecution::copy_dongmendb(string from_dir_name, string dest_dir_name){

    return copyDir(from_dir_name.c_str(), dest_dir_name.c_str());
};

int TestExecution::copy_exp_to_dongmendb(string exp_dir_name, string dest_dir_name,
                                         std::map<string, string> exp_files) {
    map<string, string>::iterator iter;
    string slash = "/";
    for(iter = exp_files.begin(); iter != exp_files.end(); iter++){
        string file_name =  exp_dir_name + slash + iter->first ;

        //        int exists = access(file_name, F_OK);
//        if (exists != 0) {
//            printf("\nerror:%s not exists.\n", exp_file_names[i]);
//            return -1;
//        }

        string dest_file_name = dest_dir_name + slash + iter->second;

        CopyFile(file_name.c_str(), dest_file_name.c_str(), FALSE);
    }

    return 0;
}

int TestExecution::cmd_cmake_refresh(string output_dir, string project_dir){

    string cmd = cmake_exe + cmake_build_type + cmake_files + project_dir;

    removeDir(output_dir.c_str());
    mkdir(output_dir.c_str());
    chdir(output_dir.c_str());

    cout<<"\n"<<cmd<<"\n";

    char result[1024] = {0};
    executeCMD(cmd, result);

    cout<<result<<"\n";

    return 0;
}

int TestExecution::cmd_cmake_clean(string build_dir_name) {

    string cmd = cmake_exe + cmd_build + build_dir_name + cmd_target_clean;

    chdir(build_dir_name.c_str());

    cout<<"\n"<<cmd<<"\n";

    char result[1024] = {0};
    executeCMD(cmd, result);
    cout<<result<<"\n";
    return 0;
}

int TestExecution::cmd_cmake_build(string build_dir_name, string exp_target) {

    string cmd = cmake_exe + cmd_build + build_dir_name + cmd_target + exp_target + cmake_others_parameters;

    chdir(build_dir_name.c_str());

    cout<<"\n"<<cmd<<"\n";

    char result[1024] = {0};

    executeCMD(cmd, result);

    cout<<result<<"\n";
    return 0;
}

int TestExecution::cmd_exp_target(string bin_dir, string exp_target) {

    chdir(bin_dir.c_str());

    cout<<"\n"<<exp_target<<"\n";

    char result[1024] = {0};

    executeCMD(exp_target, result);
    cout<<result<<"\n";
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


int TestExecution::copyDir(const char *src_dir, const char * dest_dir){
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


int TestExecution::executeCMD(string cmd, char *result)
{
    char buf_ps[1024]={0};
    FILE *ptr;
    ptr = popen(cmd.c_str(), "rt");
    if(ptr!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
            cout<<buf_ps;
//            int buf_len = strlen(buf_ps);
//            if(strlen(result) + buf_len >=1024)
//                continue;
//            else
//                strcat(result, buf_ps);
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