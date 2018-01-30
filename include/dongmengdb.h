//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_DONGMENGDB_H
#define DONGMENDB_DONGMENGDB_H

#include "filemanager.h"
#include "buffermanager.h"

#include "integritymanager.h"
#include "logmanager.h"
#include "metadatamanager.h"
#include "securitymanager.h"
#include "recordfile.h"

#define MAX_ID_NAME_LENGTH 32

/* Forward declarations.
 * From the API's perspective's, these are opaque data types. */
typedef struct dongmengdb_stmt dongmengdb_stmt;
typedef struct buffer_manager_ buffer_manager;
typedef struct file_manager_ file_manager;
typedef struct metadata_manager_ metadata_manager;

typedef struct dongmengdb_{
    char *dbName;
    file_manager *fileManager;
    buffer_manager *bufferManager;
    metadata_manager *metadataManager;
    log_manager *logManager;
    security_manager *securityManager;
    integrity_manager *integrityManager;

} dongmengdb;

/* API return codes */
#define DONGMENGDB_OK (0)
#define DONGMENGDB_EINVALIDSQL (1)
#define DONGMENGDB_ENOMEM (2)
#define DONGMENGDB_ECANTOPEN (3)
#define DONGMENGDB_ECORRUPT (4)
#define DONGMENGDB_ECONSTRAINT (5)
#define DONGMENGDB_EMISMATCH (6)
#define DONGMENGDB_EIO (7)
#define DONGMENGDB_EMISUSE (8)

#define DONGMENGDB_ROW (100)
#define DONGMENGDB_DONE (101)


/* Opens a dongmengdb file.
 *
 * If the file does not exist, it will be created
 *
 * Parameters
 * - file: Filename of the dongmengdb file to open/create
 * - db: Out parameter. Returns a pointer to a dongmengdb struct. The dongmengdb
 *       struct is an opaque type representing a dongmengdb database. In
 *       other words, an API user should not be concerned with what
 *       is contained in a variable of type dongmengdb, and should simply
 *       use it as a representation of a dongmengdb database to pass along
 *       to other API functions.
 *
 * Return
 * - DONGMENGDB_OK: Operation successful
 * - DONGMENGDB_ENOMEM: Could not allocate memory
 * - DONGMENGDB_ECANTOPEN: Unable to open the database file
 * - DONGMENGDB_ECORRUPT: The database file is not well formed
 * - DONGMENGDB_EIO: An I/O error has occurred when accessing the file
 */
int dongmengdb_open(char *file, dongmengdb *db);


/* Prepares a SQL statement for execution
 *
 * Parameters
 * - db: dongmengdb database
 * - sql: SQL statement
 * - stmt: Out parameter. Returns a pointer to a dongmengdb_stmt. The dongmengdb_stmt
 *         type is an opaque type representing a prepared SQL statement.
 *
 * Return
 * - DONGMENGDB_OK: Operation successful
 * - DONGMENGDB_EINVALIDSQL: Invalid SQL
 * - DONGMENGDB_ENOMEM: Could not allocate memory
 */
int dongmengdb_prepare(dongmengdb *db, const char *sql, dongmengdb_stmt **stmt);


/* Steps through a prepared SQL statement
 *
 * This function will run the SQL statement until a result row is available
 * or just runs the SQL statement to completion if it is not meant to
 * produce a result row (such as an INSERT statement)
 *
 * If the statement is a SELECT statement, this function returns
 * DONGMENGDB_ROW each time a result row is produced. The values of the
 * result row can be accessed using the column access functions
 * (dongmengdb_column_*). Thus, dongmengdb_step has to be called repeatedly
 * to access all the rows returned by the query. Once there are no
 * more rows left, or if the statement is not meant to produce any
 * results, then DONGMENGDB_DONE is returned (note that this function does
 * not return DONGMENGDB_OK).
 *
 * Parameters
 * - stmt: Prepared SQL statement
 *
 * Return
 * - DONGMENGDB_ROW: Statement returned a row.
 * - DONGMENGDB_DONE: Statement has finished executing.
 */
int dongmengdb_step(dongmengdb_stmt *stmt);


/* Finalizes a SQL statement, freeing all resources associated with it.
 *
 * Parameters
 * - stmt: Prepared SQL statement
 *
 * Return
 * - DONGMENGDB_OK: Operation successful
 * - DONGMENGDB_EMISUSE: Statement was already finalized
 */
int dongmengdb_finalize(dongmengdb_stmt *stmt);


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
int dongmengdb_column_count(dongmengdb_stmt *stmt);


/* Returns the type of a column
 *
 * Parameters
 * - stmt: Prepared SQL statement
 * - col: Column (columns are numbered from 0)
 *
 * Return
 * - Column type (see dongmengdb Architecture document for valid types)
 */
int dongmengdb_column_type(dongmengdb_stmt *stmt, int col);


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
const char *dongmengdb_column_name(dongmengdb_stmt* stmt, int col);


/* Returns the value of a column of integer type
 *
 * Parameters
 * - stmt: Prepared SQL statement
 * - col: Column (columns are numbered from 0)
 *
 * Return
 * - Integer value
 */
int dongmengdb_column_int(dongmengdb_stmt *stmt, int col);


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
 *   (note that this may happen after dongmengdb_step is called again)
 */
const char *dongmengdb_column_text(dongmengdb_stmt *stmt, int col);


/* Closes a dongmengdb database
 *
 * Parameters
 * - db: dongmengdb database
 *
 * Return
 * - DONGMENGDB_OK: Operation successful
 * - DONGMENGDB_EMISUSE: Database that is already closed
 */
int dongmengdb_close(dongmengdb *db);
#endif //DONGMENDB_DONGMENGDB_H
