//
// Created by Sam on 2018/2/7.
//

#include "physicalscan.h"

physical_scan *physical_scan_generate(dongmengdb *db, SRA_t *sra, transaction *tx) {
    if (!sra) return NULL;
    switch (sra->t) {
        case SRA_TABLE: {
            physical_scan *physicalScanTable = physical_scan_table_create(db, sra->table.ref->table_name, tx);
            return physicalScanTable;
        }
        case SRA_SELECT: {
            physical_scan *scan = physical_scan_generate(db, sra->select.sra, tx);
            physical_scan *physicalScanSelect = physical_scan_select_create(scan);
            physicalScanSelect->physicalScanSelect->cond = sra->select.cond;
            return physicalScanSelect;
        }
        case SRA_PROJECT: {
            physical_scan *scan = physical_scan_generate(db, sra->select.sra, tx);
            physical_scan *physicalScanProject = physical_scan_project_create(scan);
            physicalScanProject->physicalScanProject->order_by = sra->project.order_by;
            physicalScanProject->physicalScanProject->group_by = sra->project.group_by;
            physicalScanProject->physicalScanProject->distinct = sra->project.distinct;
            physicalScanProject->physicalScanProject->expr_list = sra->project.expr_list;
            return physicalScanProject;
        }
        case SRA_UNION:
            return NULL;
        case SRA_EXCEPT:
            return NULL;
        case SRA_INTERSECT:
            return NULL;
        case SRA_JOIN: {//cross join , product
            physical_scan *scan1 = physical_scan_generate(db, sra->binary.sra1, tx);
            physical_scan *scan2 = physical_scan_generate(db, sra->binary.sra2, tx);
            physical_scan *physicalScanJoin = physical_scan_join_nest_loop_create(scan1, scan2);
            physicalScanJoin->physicalScanJoinNestLoop->cond = NULL;
            return physicalScanJoin;
        }
        case SRA_NATURAL_JOIN:
            return NULL;
        case SRA_LEFT_OUTER_JOIN:
        case SRA_RIGHT_OUTER_JOIN:
        case SRA_FULL_OUTER_JOIN:
            return NULL;
        default:
            return NULL;
    }
};
