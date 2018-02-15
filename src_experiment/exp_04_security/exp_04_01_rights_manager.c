//
// Created by Sam on 2018/2/13.
//

/**
 * 解析create user | role , grant right on table | view to user
 * revoke right on table | view from user
 */

#include "dongmendb/securitymanager.h"

int security_manager_create_user(security_manager *securityManager, const char *userName, arraylist *roles, arraylist *rights ){

};

int security_manager_create_role(security_manager *securityManager, const char *roleName, arraylist *roles, arraylist *rights ){

};

int security_manager_check_right(security_manager *securityManager, const char *userName, const char *objName, right_type rightType){

};