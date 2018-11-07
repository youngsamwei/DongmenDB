//
// Created by sam on 2018/11/3.
//
#include <ctime>
#include <io.h>
#include <cstring>
#include "test_execution.h"
#include <afxres.h>
#include <iostream>
#include <assert.h>
#include <process.h>
#include <tlhelp32.h>
#include <unistd.h>
#include <thread>

using namespace std;

int  TestExecution::batchrun(wstring exp_name, wstring exp_target, wstring exp_dir_name,
             std::map<wstring, wstring> exp_files,
             wstring work_dir, wstring dongmendb_src_dir,
                             wstring output_dir){

    struct _wfinddata_t fb;   //查找相同属性文件的存储结构体

    long    handle;
    int  resultone = 0;
    int   noFile;            //对系统隐藏文件的处理标记

    handle = _wfindfirst((exp_dir_name + L"/*").c_str(),&fb);
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
                //属性值为16，则说明是文件夹，调用任务处理
                if (fb.attrib == 16)
                {
                    wstring dir = exp_dir_name + L"/" + fb.name;
                   run(exp_name, exp_target, dir,
                            exp_files, work_dir, dongmendb_src_dir, output_dir);
                }

            }
        }
        //关闭文件夹
        _findclose(handle);
    }

    return  resultone;
};

int TestExecution::run(wstring exp_name, wstring exp_target, wstring exp_dir_name,
                       std::map<wstring, wstring> exp_files,
                       wstring work_dir, wstring dongmendb_src_dir,
                       wstring output_dir) {
    wstring str = s2ws(string(rand_str(10)));
    wstring current_dir = work_dir + L"/" + project_name + L"_" + str;
    wstring build_dir = current_dir + L"/" + cmake_build_dir;
    wstring bin_dir = current_dir + L"/bin";

    int c  = init_dongmendb(work_dir, current_dir);

    copy_dongmendb(dongmendb_src_dir, current_dir);

    int pos = exp_dir_name.find_last_of(L"/");
    if (pos <0){
        pos = exp_dir_name.find_last_of(L"\\");
    }
    wstring exp_student_name = exp_dir_name.substr(pos, exp_dir_name.length());

    ofstream xout;

    /*复制文件开始*/
    string copy_log_file_name = ws2s(output_dir + L"/" + exp_student_name) + "_copy_files_failed.txt";
    xout.open(copy_log_file_name);
    assert(xout.is_open());
    /*复制实验任务的文件:*/
    int ret = copy_exp_to_dongmendb(exp_dir_name, current_dir, exp_files, xout);
    xout.close();
    xout.clear();
    if (ret < 0){
        clear_dongmendb(work_dir, current_dir);
        /*若文件不存在*/
        return -1;
    } else {
        string copy_log_file_name_passed = ws2s(output_dir + L"/" + exp_student_name) + "_copy_files_passed.txt";
        rename(copy_log_file_name.c_str(), copy_log_file_name_passed.c_str());
    }
    /*复制文件结束*/

    cmd_cmake_refresh(build_dir, current_dir);

    cmd_cmake_clean(build_dir);

    /*build开始*/
    string build_log_file_name = ws2s(output_dir + L"/" + exp_student_name) + "_build_failed.txt";
    xout.open(build_log_file_name);

    ret = cmd_cmake_build(build_dir, exp_target, xout);
    xout.close();
    xout.clear();

    /*检测exe文件是否产生，若产生则编译成功*/
    wstring bin_exe_name = bin_dir + L"/" + exp_target + L".exe";
    int exists = _waccess(bin_exe_name.c_str(), F_OK);
    if (exists != 0) {
        clear_dongmendb(work_dir, current_dir);
        return -1;
    }else{
        string build_log_file_name_passed = ws2s(output_dir + L"/" + exp_student_name) + "_build_passed.txt";
        rename(build_log_file_name.c_str(), build_log_file_name_passed.c_str());
    }
    /*build结束*/

    string test_cases_execution_log_file_name = ws2s(output_dir + L"/" + exp_student_name) + "_test_cases_execution_failed.txt";
    xout.open(test_cases_execution_log_file_name);

    string test_cased_execution_passed_flag = "[  PASSED  ]";
    wstring exe_file_name = exp_target + L".exe";
    ret = cmd_exp_target(bin_dir, exe_file_name, xout, test_cased_execution_passed_flag);
    xout.close();
    xout.clear();

    if (ret >= 0) {
        string test_cases_execution_log_file_name_passed = ws2s(output_dir + L"/" + exp_student_name) + "_test_cases_execution_passed.txt";
        rename(test_cases_execution_log_file_name.c_str(), test_cases_execution_log_file_name_passed.c_str());
    }else {
        clear_dongmendb(work_dir, current_dir);
        return -1;
    }
    clear_dongmendb(work_dir, current_dir);
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

int TestExecution::clear_dongmendb(wstring work_dir, wstring dir_name){
    cout<<"deleting "<<ws2s(dir_name);
    _wchdir(work_dir.c_str());
    removeDirW(dir_name.c_str());
    return 0;
};

int TestExecution::copy_dongmendb(wstring from_dir_name, wstring dest_dir_name){

    return copyDir(from_dir_name.c_str(), dest_dir_name.c_str());
};

int TestExecution::copy_exp_to_dongmendb(wstring exp_dir_name, wstring dest_dir_name,
                                         std::map<wstring, wstring> exp_files, ofstream& xout) {
    xout<<endl;
    xout<<ws2s(L"开始复制文件..")<<endl;
    map<wstring, wstring>::iterator iter;
    wstring slash = L"/";
    for(iter = exp_files.begin(); iter != exp_files.end(); iter++){
        wstring find_file = findFileNameEndWith(exp_dir_name, iter->first);
        if (find_file.length() == 0){
            /*若没有找到需要复制的文件*/
            xout<<ws2s(L"缺少文件:")<<ws2s(iter->first)<<endl;
            return -1;
        }
        wstring file_name =  exp_dir_name + slash + find_file ;
        wstring dest_file_name = dest_dir_name + slash + iter->second;

        CopyFileW(file_name.c_str(), dest_file_name.c_str(), FALSE);
        xout<<ws2s(L"复制 ")<<ws2s(file_name)<<"  to "<< ws2s(dest_file_name)<<endl;
    }
    xout<<ws2s(L"复制文件完成");
    xout<<endl;
    return 0;
}

int TestExecution::cmd_cmake_refresh(wstring output_dir, wstring project_dir){

    wstring cmd = cmake_exe + cmake_build_type + cmake_files + project_dir;

    removeDirW(output_dir.c_str());
    _wmkdir(output_dir.c_str());
    _wchdir(output_dir.c_str());

    cout<<endl<<ws2s(cmd)<<endl;

    executeCMD(cmd);

    return 0;
}

int TestExecution::cmd_cmake_clean(wstring build_dir_name) {

    wstring cmd = cmake_exe + cmd_build + build_dir_name + cmd_target_clean;

    _wchdir(build_dir_name.c_str());

    cout<<endl<<ws2s(cmd)<<endl;

    executeCMD(cmd);

    return 0;
}

int TestExecution::cmd_cmake_build(wstring build_dir_name, wstring exp_target, std::ofstream& xout) {

    wstring cmd = cmake_exe + cmd_build + build_dir_name + cmd_target + exp_target + cmake_others_parameters;

    _wchdir(build_dir_name.c_str());

    xout<<endl<<ws2s(cmd)<<endl;

    executeCMD(cmd, xout);

    return 0;
}

int TestExecution::cmd_exp_target(wstring bin_dir, wstring exp_target, ofstream& xout, string contents) {

    _wchdir(bin_dir.c_str());

    xout<<endl<<ws2s(exp_target)<<endl;

    return executeCMD(exp_target, xout, contents);

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


int TestExecution::copyDir(wstring src_dir, wstring dest_dir){
    struct _wfinddata_t fb;   //查找相同属性文件的存储结构体
    wstring path;
    wstring dest_path;

    long    handle;
    int  resultone = 0;
    int   noFile;             //对系统隐藏文件的处理标记

    noFile = 0;
    handle = 0;

    path = src_dir + L"/*";
    //制作路径

    handle = _wfindfirst(path.c_str(), &fb);
    //找到第一个匹配的文件
    if (handle != 0)
    {
        //当可以继续找到匹配的文件，继续执行
        while (0 == _wfindnext(handle,&fb))
        {
            //windows下，常有个系统文件，名为“..”,对它不做处理
            noFile = wcscmp(fb.name,L"..");

            if (0 != noFile)
            {
                //制作完整路径
                dest_path = dest_dir + L"/" + fb.name;
                path = src_dir + L"/" + fb.name;

                //属性值为16，则说明是文件夹，迭代
                if (fb.attrib == 16)
                {
                    _wmkdir(dest_path.c_str());
                    copyDir(path, dest_path);
                }  //非文件夹的文件，直接复制。对文件属性值的情况没做详细调查，可能还有其他情况。
                else
                {
                    CopyFileW(path.c_str(), dest_path.c_str(), FALSE);
                }
            }
        }
        //关闭文件夹。找这个函数找了很久，标准c中用的是closedir
        //经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。
        _findclose(handle);
    }
    return  resultone;
}

wstring  TestExecution::findFileNameEndWith(wstring dir, wstring name){
    struct _wfinddata_t fb;   //查找相同属性文件的存储结构体
    wstring path;
    wstring resultone = L"";

    long    handle;
    int   noFile;             //对系统隐藏文件的处理标记

    noFile = 0;
    handle = 0;

    path = dir + L"/*";
    //制作路径

    handle = _wfindfirst(path.c_str(), &fb);
    //找到第一个匹配的文件
    if (handle != 0)
    {
        //当可以继续找到匹配的文件，继续执行
        while (0 == _wfindnext(handle,&fb))
        {
            //属性值为16，则说明是文件夹
            if (fb.attrib != 16){
                wstring fb_name = fb.name;
                if (fb_name.length() >= name.length()){
                    wstring px = fb_name.substr(fb_name.length()- name.length(), fb_name.length());
                    int r = wcscmp(px.c_str(), name.c_str());
                    if (r == 0){
                        resultone = fb.name;
                        break;
                    }
                }
            }

        }
        //关闭文件夹。找这个函数找了很久，标准c中用的是closedir
        //经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。
        _findclose(handle);
    }
    return  resultone;
};

void find_process_and_kill(const char* strFilename) {
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE){
        return ;
    }

    PROCESSENTRY32 pe32 = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcessSnap, &pe32)){
        do{

            if(strcmp(pe32.szExeFile, strFilename) == 0){
                HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);
                TerminateProcess(hprocess, 0);
                CloseHandle(hprocess);
                break;
            }
        }
        while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
}
/*监控执行测试用例的exe程序是否出现异常导致程序无法终止。
 * 参数：
 * exe_file_name ：被监控的进程名称
 * timeout：超时阈值，以秒为单位*/

void monitor_test_cases_execution(string exe_file_name, int timeout, int& recieving_chars_count ){

    int prev_receiving_chars = -1;
    clock_t prev_time;
    prev_time=clock();
    while (recieving_chars_count >= 0){
        sleep(1);
        if (prev_receiving_chars == recieving_chars_count){
            if ((clock() - prev_time) > timeout * 1000) {
                find_process_and_kill(exe_file_name.c_str());
                /*终止进程*/
                cout<<endl<<exe_file_name<<" will be stopped."<<endl;
                break;
            }

        }else{
            prev_time=clock();
            prev_receiving_chars = recieving_chars_count;
        }

    }

    /*关闭弹出的appcrash窗口，如果有的话*/
    const char* werfault= "WerFault.exe";
    find_process_and_kill(werfault);
}

int TestExecution::executeCMD(wstring cmd, ofstream& xout, string contents)
{
    char buf_ps[1024]={0};
    FILE *ptr;
    ptr = _wpopen(cmd.c_str(), L"rt");
    if(ptr!=NULL)
    {
        recieving_chars_count = 0;
        string exe_file_name = ws2s(cmd).c_str();
        int tm = 10;
        std::thread t(monitor_test_cases_execution, exe_file_name, tm, std::ref(recieving_chars_count));

        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
            xout << buf_ps;
            recieving_chars_count++;
        }
        pclose(ptr);

        t.join();
        recieving_chars_count = -1;


        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", cmd);
    }
    string last_buf(buf_ps);

    return last_buf.find(contents);
}

int TestExecution::executeCMD(wstring cmd)
{
    char buf_ps[1024]={0};
    FILE *ptr;
    ptr = _wpopen(cmd.c_str(), L"rt");
    if(ptr!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
                cout<<buf_ps;
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

int TestExecution::executeCMD(wstring cmd, ofstream& xout)
{
    char buf_ps[1024]={0};
    FILE *ptr;
    ptr = _wpopen(cmd.c_str(), L"rt");
    if(ptr!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
            xout << buf_ps;
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


string  TestExecution::ws2s(const wstring& ws)
{
    string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

    setlocale(LC_ALL, "chs");

    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    string result = _Dest;
    delete []_Dest;

    setlocale(LC_ALL, curLocale.c_str());

    return result;
}

wstring  TestExecution::s2ws(const string& s)
{
    setlocale(LC_ALL, "chs");

    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    wstring result = _Dest;
    delete []_Dest;

    setlocale(LC_ALL, "C");

    return result;
}


