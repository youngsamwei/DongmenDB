//
// Created by sam on 2018/11/15.
//

#ifndef DONGMENDB_PLAN_H
#define DONGMENDB_PLAN_H

#include <dongmendb/dongmendb.h>
#include <dongmensql/sra.h>
#include <physicalplan/Scan.h>

class Plan {
    public :
    Scan generateSelect(dongmendb *db, SRA_t *sra, transaction *tx);
    Scan generateScan(dongmendb *db, SRA_t *sra, transaction *tx);

};
#endif //DONGMENDB_PLAN_H
