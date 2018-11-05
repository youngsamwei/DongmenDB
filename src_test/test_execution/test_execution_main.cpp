//
// Created by sam on 2018/11/4.
//

#include "test_execution.h"


int main(int argc, char *argv[]) {

    string exp_name = "experiment_3";
    string exp_target = "exp_01_04_update_test";
    string exp_dir_name = "F:/dongmendb_exp/201401060413";
    string work_dir = "F:/dongmendb";
    string dongmendb_src_dir = "E:/CLion_workspace/DongmenDB";

    std::map<string, string> exp_files;
    exp_files["exp_01_04_update.c"] = "/src_experiment/exp_01_stmt_parser/exp_01_04_update.c";
    exp_files["exp_07_05_update.c"] = "/src_experiment/exp_07_physical_operate/exp_07_05_update.c";

    TestExecution te;
    te.run(exp_name, exp_target, exp_dir_name, exp_files, work_dir, dongmendb_src_dir);

//    te.run(exp_name, exp_target, exp_dir_name, NULL, NULL, exp_file_len,
//           work_dir, dongmendb_src_dir);

}