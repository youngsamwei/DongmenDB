//
// Created by Sam on 2018/2/13.
//

/**
 * 在系统表user,role, userrole, userright, roleright中创建用户，创建角色，授权，收回授权，权限检查
 *
 */

#include "dongmendb/securitymanager.h"

int SecurityManager::security_manager_create_user(const char *userName) {

};

int SecurityManager::security_manager_create_role(const char *roleName) {

};

int SecurityManager::security_manager_check_right(const char *userName, const char *objName,
                                 right_type rightType) {

};

int SecurityManager::security_manager_grant_role_to_user(const char *userName, const char *roleName) {

};

int SecurityManager::security_manager_grant_right_to_user(const char *userName, const char *objName,
                                         right_type rightType) {

};

int SecurityManager::security_manager_grant_right_to_role(const char *roleName, const char *objName,
                                         right_type rightType) {

};

int SecurityManager::security_manager_revoke_role_from_user(const char *userName,
                                           const char *roleName) {

};

int SecurityManager::security_manager_revoke_right_from_user(const char *userName, const char *objName,
                                        right_type rightType) {

};

int SecurityManager::security_manager_revoke_right_from_role(const char *roleName, const char *objName,
                                        right_type rightType) {

};
