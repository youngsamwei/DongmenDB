//
// Created by sam on 2018/11/4.
//

#include <iostream>
#include "test_execution.h"
#include <locale.h>

int main(int argc, char *argv[]) {
    locale::global(locale(""));

    wstring exp_name = L"experiment_3";
    wstring exp_target = L"exp_01_04_update_test";
    wstring exp_dir_name = L"F:\\云班课作业 2018\\计算机16-1，2，3-数据库系统-课程设计_实验2_实现u\\201401060413_刘子笛";
    wstring a= L"云班课作业";
    wstring work_dir = L"F:/dongmendb";
    wstring dongmendb_src_dir = L"E:/CLion_workspace/DongmenDB";

    std::map<wstring, wstring> exp_files;
    exp_files[L"exp_01_04_update.c"] = L"/src_experiment/exp_01_stmt_parser/exp_01_04_update.c";
    exp_files[L"exp_07_05_update.c"] = L"/src_experiment/exp_07_physical_operate/exp_07_05_update.c";

    TestExecution te;
    te.run(exp_name, exp_target, exp_dir_name, exp_files, work_dir, dongmendb_src_dir);

//    te.run(exp_name, exp_target, exp_dir_name, NULL, NULL, exp_file_len,
//           work_dir, dongmendb_src_dir);

}