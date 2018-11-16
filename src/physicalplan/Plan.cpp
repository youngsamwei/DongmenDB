//
// Created by sam on 2018/11/15.
//

#include <physicalplan/Plan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

Scan* Plan::generateSelect(dongmendb *db, SRA_t *sra, transaction *tx){
    Scan *plan = generateScan(db, sra, tx);
//    if (typeid(*plan) == typeid(Project)) {
        /*处理select 中形如 student.* */
//        physical_scan_project_generate_expr_list(plan);
//    }
    return plan;
};

Scan* Plan::generateScan(dongmendb *db, SRA_t *sra, transaction *tx){
    if (!sra) return NULL;
    switch (sra->t) {
        case SRA_TABLE: {
            TableScan tableScan(db, sra->table.ref->table_name, tx);
            return &tableScan;
        }
        case SRA_SELECT: {
            Scan *scan = this->generateScan(db, sra->select.sra, tx);
            Select select(scan);
            select.cond = sra->select.cond;
            return &select;
        }
        case SRA_PROJECT: {
            Scan *scan = this->generateScan(db, sra->select.sra, tx);
            Project project(scan);

            project.order_by = sra->project.order_by;
            project.group_by = sra->project.group_by;
            project.distinct = sra->project.distinct;
            /*处理 形如student.*的字段表示 */
            project.original_expr_list = sra->project.expr_list;
            project.expr_list = arraylist_create();

            return &project;
        }
        case SRA_UNION:
            return NULL;
        case SRA_EXCEPT:
            return NULL;
        case SRA_INTERSECT:
            return NULL;
        case SRA_JOIN: {//cross join , product
            Scan *scan1 = this->generateScan(db, sra->binary.sra1, tx);
            Scan *scan2 = this->generateScan(db, sra->binary.sra2, tx);
            Join join(scan1, scan2);
            join.cond = NULL;
            return &join;
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

