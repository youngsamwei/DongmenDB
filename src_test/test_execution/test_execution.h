//
// Created by sam on 2018/11/3.
//

#ifndef DONGMENDB_TEST_EXECUTION_H
#define DONGMENDB_TEST_EXECUTION_H

#include <cstddef>
#include <utils/utils.h>
#include <string>
#include <map>
#include <list>

using namespace std;


class TestExecution {

    wstring project_name = L"dongmendb";
    wstring cmake_build_type = L" -DCMAKE_BUILD_TYPE=Debug ";
    wstring cmake_files = L"-G \"CodeBlocks - MinGW Makefiles\" ";
    wstring cmake_build_dir = L"cmake-build-debug";
    wstring cmd_build  = L" --build ";
    wstring cmd_target_clean = L" --target clean -- -j 2 ";
    wstring cmd_target = L" --target ";
    wstring cmake_others_parameters = L" -- -j 2";
    wstring cmake_exe = L"cmake.exe ";
    const int SIZE_RAND_STR_LEN = 32; //生成32 + 1位C Style字符串
    const char *CCH = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

public:
    /*执行实验测试
     * 参数：实验名称，编译目标，实验作业所在文件夹*/
    int run(wstring exp_name, wstring exp_target, wstring exp_dir_name,
            std::map<wstring, wstring> exp_files,
            wstring work_dir, wstring dongmendb_src_dir);

protected:
    /*初始化dongmendb文件夹*/
    int init_dongmendb(wstring work_dir, wstring dir_name);

    /*复制dongmendb的源代码到目标文件夹*/
    int copy_dongmendb(wstring from_dir_name, wstring dest_dir_name);

    /*从实验作业文件夹复制文件到目标文件夹*/
    int copy_exp_to_dongmendb(wstring exp_dir_name, wstring dest_dir_name,
                              std::map<wstring, wstring> exp_files);

    int cmd_cmake_refresh(wstring output_dir, wstring build_dir_name);
    /*在cmd中执行命令，清理cmake产生的文件*/
    int cmd_cmake_clean(wstring build_dir_name);

    /*在cmd中执行命令，build代码*/
    int cmd_cmake_build(wstring build_dir_name, wstring exp_target);

    /*在cmd中执行命令，运行exe目标文件*/
    int cmd_exp_target(wstring bin_dir, wstring exp_target);

    /*获取测试结果*/
    int cmd_get_test_result();

private:
    /*产生指定长度的随机字符串, 默认长度32*/
    wchar_t *rand_str(size_t len);
    wchar_t *rand_str();
    int copyDir(wstring src_dir,wstring dest_dir);

public:
    /*执行cmd命令，并将执行结果写入result*/
    int executeCMD(wstring cmd, char *result);
    int executeCMD();
};

#endif //DONGMENDB_TEST_EXECUTION_H
