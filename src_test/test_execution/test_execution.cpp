//
// Created by sam on 2018/11/3.
//
#include <ctime>
#include <io.h>
#include <cstring>
#include "test_execution.h"
#include <afxres.h>
#include <iostream>

int TestExecution::run(wstring exp_name, wstring exp_target, wstring exp_dir_name,
                       std::map<wstring, wstring> exp_files,
                       wstring work_dir, wstring dongmendb_src_dir) {
    wstring str = rand_str(10);
    wstring current_dir = work_dir + L"/" + project_name + L"_" + str;
    wstring build_dir = current_dir + L"/" + cmake_build_dir;
    wstring bin_dir = current_dir + L"/bin";

    int c  = init_dongmendb(work_dir, current_dir);

    copy_dongmendb(dongmendb_src_dir, current_dir);

    /*������ҵ������Ŀ¼*/
    int ret = copy_exp_to_dongmendb(exp_dir_name, current_dir, exp_files);

    /*ȱ��ʵ�������ļ�*/
    if (ret < 0){
        return -1;
    }

    cmd_cmake_refresh(build_dir, current_dir);

    cmd_cmake_clean(build_dir);

    cmd_cmake_build(build_dir, exp_target);

    cmd_exp_target(bin_dir, exp_target);

    return 0;
}

int TestExecution::init_dongmendb(wstring work_dir, wstring dir_name) {
    _wchdir(work_dir.c_str());
    _wmkdir(dir_name.c_str());

    int exists = _waccess(dir_name.c_str(), F_OK);
    if (exists != 0) {
        return -1;
    }
    return 0;
}

int TestExecution::copy_dongmendb(wstring from_dir_name, wstring dest_dir_name){

    return copyDir(from_dir_name.c_str(), dest_dir_name.c_str());
};

int TestExecution::copy_exp_to_dongmendb(wstring exp_dir_name, wstring dest_dir_name,
                                         std::map<wstring, wstring> exp_files) {
    cout<<"\n";
    map<wstring, wstring>::iterator iter;
    wstring slash = L"/";
    for(iter = exp_files.begin(); iter != exp_files.end(); iter++){
        wstring file_name =  exp_dir_name + slash + iter->first ;

        //        int exists = access(file_name, F_OK);
//        if (exists != 0) {
//            printf("\nerror:%s not exists.\n", exp_file_names[i]);
//            return -1;
//        }

        wstring dest_file_name = dest_dir_name + slash + iter->second;

        CopyFileW(file_name.c_str(), dest_file_name.c_str(), FALSE);
        wcout<<"copy "<<file_name<<"  to "<< dest_file_name<<endl;
    }

    cout<<"\n";
    return 0;
}

int TestExecution::cmd_cmake_refresh(wstring output_dir, wstring project_dir){

    wstring cmd = cmake_exe + cmake_build_type + cmake_files + project_dir;

    removeDirW(output_dir.c_str());
    _wmkdir(output_dir.c_str());
    _wchdir(output_dir.c_str());

    cout<<"\n"<<cmd.c_str()<<"\n";

    char result[1024] = {0};
    executeCMD(cmd, result);

    cout<<result<<"\n";

    return 0;
}

int TestExecution::cmd_cmake_clean(wstring build_dir_name) {

    wstring cmd = cmake_exe + cmd_build + build_dir_name + cmd_target_clean;

    _wchdir(build_dir_name.c_str());

    cout<<"\n"<<cmd.c_str()<<"\n";

    char result[1024] = {0};
    executeCMD(cmd, result);
    cout<<result<<"\n";
    return 0;
}

int TestExecution::cmd_cmake_build(wstring build_dir_name, wstring exp_target) {

    wstring cmd = cmake_exe + cmd_build + build_dir_name + cmd_target + exp_target + cmake_others_parameters;

    _wchdir(build_dir_name.c_str());

    cout<<"\n"<<cmd.c_str()<<"\n";

    char result[1024] = {0};

    executeCMD(cmd, result);

    cout<<result<<"\n";
    return 0;
}

int TestExecution::cmd_exp_target(wstring bin_dir, wstring exp_target) {

    _wchdir(bin_dir.c_str());

    cout<<"\n"<<exp_target.c_str()<<"\n";

    char result[1024] = {0};

    executeCMD(exp_target, result);
    cout<<result<<"\n";
    return 0;
}

int TestExecution::cmd_get_test_result() {
    return 0;
}

wchar_t *TestExecution::rand_str(size_t len) {
    srand((unsigned)time(NULL));
    wchar_t *ch = (wchar_t *)malloc((len + 1)*sizeof(wchar_t*));
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

wchar_t *TestExecution::rand_str() {
    return rand_str(SIZE_RAND_STR_LEN);
}


int TestExecution::copyDir(wstring src_dir, wstring dest_dir){
    struct _wfinddata_t fb;   //������ͬ�����ļ��Ĵ洢�ṹ��
    wstring path;
    wstring dest_path;

    long    handle;
    int  resultone = 0;
    int   noFile;            //��ϵͳ�����ļ��Ĵ�����

    noFile = 0;
    handle = 0;

    path = src_dir + L"/*";
    //����·��

    handle = _wfindfirst(path.c_str(), &fb);
    //�ҵ���һ��ƥ����ļ�
    if (handle != 0)
    {
        //�����Լ����ҵ�ƥ����ļ�������ִ��
        while (0 == _wfindnext(handle,&fb))
        {
            //windows�£����и�ϵͳ�ļ�����Ϊ��..��,������������
            noFile = wcscmp(fb.name,L"..");

            if (0 != noFile)
            {
                //��������·��
                dest_path = dest_dir + L"/" + fb.name;
                path = src_dir + L"/" + fb.name;

                //����ֵΪ16����˵�����ļ��У�����
                if (fb.attrib == 16)
                {
                    _wmkdir(dest_path.c_str());
                    copyDir(path, dest_path);
                }  //���ļ��е��ļ���ֱ�Ӹ��ơ����ļ�����ֵ�����û����ϸ���飬���ܻ������������
                else
                {
                    CopyFileW(path.c_str(), dest_path.c_str(), FALSE);
                }
            }
        }
        //�ر��ļ��С�������������˺ܾã���׼c���õ���closedir
        //������ܣ�һ�����Handle�ĺ���ִ�к󣬶�Ҫ���йرյĶ�����
        _findclose(handle);
    }
    return  resultone;
}


int TestExecution::executeCMD(wstring cmd, char *result)
{
    char buf_ps[1024]={0};
    FILE *ptr;
    ptr = _wpopen(cmd.c_str(), L"rt");
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