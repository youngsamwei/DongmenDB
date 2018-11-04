//
// Created by sam on 2018/11/3.
//

#ifndef DONGMENDB_TEST_EXECUTION_H
#define DONGMENDB_TEST_EXECUTION_H

#include <cstddef>
#include <utils/utils.h>

class TestExecution {

    const char *cmake_exe = "\"D:/Program Files/JetBrains/CLion 2017.3.1/bin/cmake/bin/cmake.exe\" ";
    char *current_dir;
    const int SIZE_RAND_STR_LEN = 32; //生成32 + 1位C Style字符串
    const char *CCH = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

public:
    /*执行实验测试
     * 参数：实验名称，编译目标，实验作业所在文件夹*/
    int run(const char *exp_name, const char *exp_target, const char *exp_dir_name, const char *work_dir, const char *dongmendb_src_dir);

protected:
    /*初始化dongmendb文件夹*/
    int init_dongmendb(const char *work_dir, const char *dir_name);

    /*复制dongmendb的源代码到目标文件夹*/
    int copy_dongmendb(const char *from_dir_name, const char *dest_dir_name);

    /*从实验作业文件夹复制文件到目标文件夹*/
    int copy_exp_to_dongmendb(const char *from_dir_name, const char *dest_dir_name);

    /*在cmd中执行命令，清理cmake产生的文件*/
    int cmd_cmake_clean(const char *build_dir_name);

    /*在cmd中执行命令，build代码*/
    int cmd_cmake_build(const char *build_dir_name, const char *exp_target);

    /*在cmd中执行命令，运行exe目标文件*/
    int cmd_exp_target(const char *build_dir_name, const char *exp_target);

    /*获取测试结果*/
    int cmd_get_test_result();

private:
    /*产生指定长度的随机字符串, 默认长度32*/
    char *rand_str(size_t len);
    char *rand_str();
    int copyDir(const char *src_dir, const char *dest_dir);

    /*执行cmd命令，并将执行结果写入result*/
    int executeCMD(const char *cmd, char *result);
};

#endif //DONGMENDB_TEST_EXECUTION_H
