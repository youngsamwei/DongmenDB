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

    string project_name = "dongmendb";
    string cmake_build_type = " -DCMAKE_BUILD_TYPE=Debug ";
    string cmake_files = "-G \"CodeBlocks - MinGW Makefiles\" ";
    string cmake_build_dir = "cmake-build-debug";
    string cmd_build  = " --build ";
    string cmd_target_clean = " --target clean -- -j 2 ";
    string cmd_target = " --target ";
    string cmake_others_parameters = " -- -j 2";
    string cmake_exe = "cmake.exe ";
    const int SIZE_RAND_STR_LEN = 32; //生成32 + 1位C Style字符串
    const char *CCH = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

public:
    /*执行实验测试
     * 参数：实验名称，编译目标，实验作业所在文件夹*/
    int run(string exp_name, string exp_target, string exp_dir_name,
            std::map<string, string> exp_files,
            string work_dir, string dongmendb_src_dir);

protected:
    /*初始化dongmendb文件夹*/
    int init_dongmendb(string work_dir, string dir_name);

    /*复制dongmendb的源代码到目标文件夹*/
    int copy_dongmendb(string from_dir_name, string dest_dir_name);

    /*从实验作业文件夹复制文件到目标文件夹*/
    int copy_exp_to_dongmendb(string exp_dir_name, string dest_dir_name,
                              std::map<string, string> exp_files);

    int cmd_cmake_refresh(string output_dir, string build_dir_name);
    /*在cmd中执行命令，清理cmake产生的文件*/
    int cmd_cmake_clean(string build_dir_name);

    /*在cmd中执行命令，build代码*/
    int cmd_cmake_build(string build_dir_name, string exp_target);

    /*在cmd中执行命令，运行exe目标文件*/
    int cmd_exp_target(string bin_dir, string exp_target);

    /*获取测试结果*/
    int cmd_get_test_result();

private:
    /*产生指定长度的随机字符串, 默认长度32*/
    char *rand_str(size_t len);
    char *rand_str();
    int copyDir(const char * src_dir, const char * dest_dir);

public:
    /*执行cmd命令，并将执行结果写入result*/
    int executeCMD(string cmd, char *result);
    int executeCMD();
};

#endif //DONGMENDB_TEST_EXECUTION_H
