//
// Created by sam on 2018/11/4.
//

#include <io.h>
#include "test_execution.h"

int main(int argc, char *argv[]) {

    const char *exp_name = "experiment_3";
    const char *exp_target = "exp_01_03_select_test";
    const char *exp_dir_name = "";
    const char *work_dir = "F:/dongmendb";
    const char *dongmendb_src_dir = "E:/CLion_workspace/DongmenDB";

    TestExecution te;
    te.run(exp_name, exp_target, exp_dir_name, work_dir, dongmendb_src_dir);


}