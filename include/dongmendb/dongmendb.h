//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_DONGMENDB_H
#define DONGMENDB_DONGMENDB_H

#include "filemanager.h"
#include "buffermanager.h"

#include "integritymanager.h"
#include "logmanager.h"
#include "metadatamanager.h"
#include "recordfile.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ID_NAME_LENGTH 32

/* Forward declarations.
 * From the API's perspective's, these are opaque data types. */

typedef struct dongmendb_stmt dongmendb_stmt;
typedef struct security_manager_ security_manager;
typedef struct buffer_manager_ buffer_manager;
typedef struct file_manager_ file_manager;
typedef struct integrity_manager_ integrity_manager;
typedef struct metadata_manager_ metadata_manager;
typedef struct transaction_ transaction;

typedef struct dongmendb_ {
    char *dbName;
    file_manager *fileManager;
    buffer_manager *bufferManager;
    metadata_manager *metadataManager;
    log_manager *logManager;
    security_manager *securityManager;
    integrity_manager *integrityManager;
    transaction *tx;
} dongmendb;

/* API return codes */
#define DONGMENDB_OK (0)
#define DONGMENDB_EINVALIDSQL (1)
#define DONGMENDB_ENOMEM (2)
#define DONGMENDB_ECANTOPEN (3)
#define DONGMENDB_ECORRUPT (4)
#define DONGMENDB_ECONSTRAINT (5)
#define DONGMENDB_EMISMATCH (6)
#define DONGMENDB_ERROR_IO (7)
#define DONGMENDB_EMISUSE (8)

#define DONGMENDB_ROW (100)
#define DONGMENDB_DONE (101)


/* Opens a dongmendb file.
 *
 * If the file does not exist, it will be created
 *
 * Parameters
 * - file: Filename of the dongmendb file to open/create
 * - db: Out parameter. Returns a pointer to a dongmendb struct. The dongmendb
 *       struct is an opaque type representing a dongmendb database. In
 *       other words, an API user should not be concerned with what
 *       is contained in a variable of type dongmendb, and should simply
 *       use it as a representation of a dongmendb database to pass along
 *       to other API functions.
 *
 * Return
 * - DONGMENDB_OK: Operation successful
 * - DONGMENDB_ENOMEM: Could not allocate memory
 * - DONGMENDB_ECANTOPEN: Unable to open the database file
 * - DONGMENDB_ECORRUPT: The database file is not well formed
 * - DONGMENDB_EIO: An I/O error has occurred when accessing the file
 */
int dongmendb_open(const char *file, dongmendb *db);


/* Prepares a SQL statement for execution
 *
 * Parameters
 * - db: dongmendb database
 * - sql: SQL statement
 * - stmt: Out parameter. Returns a pointer to a dongmendb_stmt. The dongmendb_stmt
 *         type is an opaque type representing a prepared SQL statement.
 *
 * Return
 * - DONGMENDB_OK: Operation successful
 * - DONGMENDB_EINVALIDSQL: Invalid SQL
 * - DONGMENDB_ENOMEM: Could not allocate memory
 */
int dongmendb_prepare(dongmendb *db, const char *sql, dongmendb_stmt **stmt);


/* Steps through a prepared SQL statement
 *
 * This function will run the SQL statement until a result row is available
 * or just runs the SQL statement to completion if it is not meant to
 * produce a result row (such as an INSERT statement)
 *
 * If the statement is a SELECT statement, this function returns
 * DONGMENDB_ROW each time a result row is produced. The values of the
 * result row can be accessed using the column access functions
 * (dongmendb_column_*). Thus, dongmendb_step has to be called repeatedly
 * to access all the rows returned by the dongmensql. Once there are no
 * more rows left, or if the statement is not meant to produce any
 * results, then DONGMENDB_DONE is returned (note that this function does
 * not return DONGMENDB_OK).
 *
 * Parameters
 * - stmt: Prepared SQL statement
 *
 * Return
 * - DONGMENDB_ROW: Statement returned a row.
 * - DONGMENDB_DONE: Statement has finished executing.
 */
int dongmendb_step(dongmendb_stmt *stmt);


/* Finalizes a SQL statement, freeing all resources associated with it.
 *
 * Parameters
 * - stmt: Prepared SQL statement
 *
 * Return
 * - DONGMENDB_OK: Operation successful
 * - DONGMENDB_EMISUSE: Statement was already finalized
 */
int dongmendb_finalize(dongmendb_stmt *stmt);


/* Returns the number of columns returned by a SQL statement
 *
 * Parameters
 * - stmt: Prepared SQL statement
 *
 * Return
 * - Number of columns in the result rows. If the SQL statement is not
 *   meant to produce any results (such as an INSERT statement), then 0
 *   is returned.
 */
int dongmendb_column_count(dongmendb_stmt *stmt);


/* Returns the type of a column
 *
 * Parameters
 * - stmt: Prepared SQL statement
 * - col: Column (columns are numbered from 0)
 *
 * Return
 * - Column type (see dongmendb Architecture document for valid types)
 */
int dongmendb_column_type(dongmendb_stmt *stmt, int col);


/* Returns the name of a column
 *
 * Parameters
 * - stmt: Prepared SQL statement
 * - col: Column (columns are numbered from 0)
 *
 * Return
 * - Pointer to a null-terminated string with the name of column. The API
 *   client does not have to free() the returned string. It is the API's
 *   responsibility to allocate and free the memory for this string.
 */
const char *dongmendb_column_name(dongmendb_stmt *stmt, int col);


/* Returns the value of a column of integer type
 *
 * Parameters
 * - stmt: Prepared SQL statement
 * - col: Column (columns are numbered from 0)
 *
 * Return
 * - Integer value
 */
int dongmendb_column_int(dongmendb_stmt *stmt, int col);


/* Returns the value of a column of string type
 *
 * Parameters
 * - stmt: Prepared SQL statement
 * - col: Column (columns are numbered from 0)
 *
 * Return
 * - Pointer to a null-terminated string with the value. The API client
 *   does not have to free() the returned string. It is the API's
 *   responsibility to allocate and free the memory for this string
 *   (note that this may happen after dongmendb_step is called again)
 */
const char *dongmendb_column_text(dongmendb_stmt *stmt, int col);


/* Closes a dongmendb database
 *
 * Parameters
 * - db: dongmendb database
 *
 * Return
 * - DONGMENDB_OK: Operation successful
 * - DONGMENDB_EMISUSE: Database that is already closed
 */
int dongmendb_close(dongmendb *db);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_DONGMENDB_H
