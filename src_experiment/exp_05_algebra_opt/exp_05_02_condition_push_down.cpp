//
// Created by Sam on 2018/2/13.
//

#include <relationalalgebra/optimizer.h>

/**
 * 使用关于选择的等价变换规则将条件下推。
 *
 */


/*输入一个关系代数表达式，输出优化后的关系代数表达式
 * 要求：在查询条件符合合取范式的前提下，根据等价变换规则将查询条件移动至合适的位置。
 * */
SRA_t *
dongmengdb_algebra_optimize_condition_pushdown(SRA_t *sra, TableManager *tableManager, Transaction *transaction) {

    /*初始关系代数语法树sra由三个操作构成：SRA_PROJECT -> SRA_SELECT -> SRA_JOIN，即对应语法树中三个节点。*/

    /*第一步：.等价变换：将SRA_SELECT类型的节点进行条件串接*/

        /*1.1 在sra中找到每个SRA_Select节点 */
        /*1.2 检查每个SRA_Select节点中的条件是不是满足串接条件：多条件且用and连接*/
        /*1.3 若满足串接条件则：创建一组新的串接的SRA_Select节点，等价替换当前的SRA_Select节点*/

    /*第二步：等价变换：条件交换*/
        /*2.1 在sra中找到每个SRA_Select节点*/
        /*2.2 对每个SRA_Select节点做以下处理：
         * 在sra中查找 SRA_Select 节点应该在的最优位置：
         *     若子操作也是SRA_Select，则可交换；
         *     若子操作是笛卡尔积，则可交换，需要判断SRA_Select所包含的属性属于笛卡尔积的哪个子操作
         * 最后将SRA_Select类型的节点移动到语法树的最优位置。
         * */
    return sra;
}