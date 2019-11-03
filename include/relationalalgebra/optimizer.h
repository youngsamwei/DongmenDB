//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_OPTIMIZER_H
#define DONGMENDB_OPTIMIZER_H


#include <relationalalgebra/sra.h>
#include <dongmendb/MetadataManager.h>

/*作为实验任务在src_experiment\exp_05_algebra_opt\exp_05_02_condition_push_down.c中实现*/
SRA_t *
dongmengdb_algebra_optimize_condition_pushdown(SRA_t *sra, TableManager *tableManager, Transaction *transaction);



#endif //DONGMENDB_OPTIMIZER_H
