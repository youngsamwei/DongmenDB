//
// Created by Sam on 2018/2/13.
//

/**
 * 在系统表user,role, userrole, userright, roleright中创建用户，创建角色，授权，收回授权，权限检查
 *
 */

#include "dongmendb/securitymanager.h"

int security_manager_create_user(security_manager *securityManager, const char *userName) {

};

int security_manager_create_role(security_manager *securityManager, const char *roleName) {

};

int security_manager_check_right(security_manager *securityManager, const char *userName, const char *objName,
                                 right_type rightType) {

};

int security_manager_grant_role_to_user(security_manager *securityManager, const char *userName, const char *roleName) {

};

int security_manager_grant_right_to_user(security_manager *securityManager, const char *userName, const char *objName,
                                         right_type rightType) {

};

int security_manager_grant_right_to_role(security_manager *securityManager, const char *roleName, const char *objName,
                                         right_type rightType) {

};

int security_manager_revoke_role_from_user(security_manager *securityManager, const char *userName,
                                           const char *roleName) {

};

int security_manager_revoke_right_from_user(security_manager *securityManager, const char *userName, const char *objName,
                                        right_type rightType) {

};

int security_manager_revoke_right_from_role(security_manager *securityManager, const char *roleName, const char *objName,
                                        right_type rightType) {

};
