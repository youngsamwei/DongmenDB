//
// Created by Sam on 2018/2/13.
//

/*
 * 检查是否存在数据表
 * 此函数在以下情况会被调用：
 * 1. 执行select，update，insert，delete时调用，检查表是否存在
 * 2 执行create table语句时检查表是否已经存在
 * 3 执行create index时检查表是否存在
 *
 * */