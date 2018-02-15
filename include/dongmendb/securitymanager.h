//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_SECURITYMANAGER_H
#define DONGMENDB_SECURITYMANAGER_H
/**
 * 安全管理
 * 安全性管理：用户，对象，权限
 * 数据保存在元数据表中。
 *
 */
#include "utils/arraylist.h"
#include "dongmendb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    RIGHT_SELECT = 0,
    RIGHT_INSERT,
    RIGHT_UPDATE,
    RIGHT_DELETE
}right_type;

typedef struct security_manager_ {

} security_manager;

typedef struct user_{
    char *userName;
    arraylist *roles;
    arraylist *rights;
}user;

typedef struct right_{
    char *objectName;
    right_type rightType;
}right;

typedef struct role_{
    char *roleName;
    arraylist *roles;
    arraylist *rights;
}role;

/**
 *  在 exp_04_01_rights_manager.c 实现
 * @param userName
 * @param roles
 * @param rights
 * @return
 */
int security_manager_create_user(security_manager *securityManager, const char *userName, arraylist *roles, arraylist *rights );
/**
 *  在 exp_04_01_rights_manager.c 实现
 * @param roleName
 * @param roles
 * @param rights
 * @return
 */
int security_manager_create_role(security_manager *securityManager, const char *roleName, arraylist *roles, arraylist *rights );

/**
 *  在 exp_04_01_rights_manager.c 中实现
 * @param userName
 * @param objName
 * @param rightType
 * @return
 */
int security_manager_check_right(security_manager *securityManager, const char *userName, const char *objName, right_type rightType);

/**
 * 在 securitymanager.c 中实现
 * @param userName
 * @return
 */
user *security_manager_get_user(security_manager *securityManager, const char *userName);

/**
 * 在 securitymanager.c 实现
 * @param roleName
 * @return
 */
role *security_manager_get_role(security_manager *securityManager, const char *roleName);

/**
 * 初始化用于安全管理的系统数据表.
 * 在 securitymanager.c 中实现
 * @param db
 * @return
 */
int security_manager_init(security_manager *securityManager);

/**
 * 在 securitymanager.c 中实现
 * @param db
 * @return
 */
security_manager *security_manager_create(dongmendb *db);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_SECURITYMANAGER_H
