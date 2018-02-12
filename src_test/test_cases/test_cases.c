//
// Created by Sam on 2018/2/12.
//

#include <mem.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    if (argc <= 4){
        return 0;
    }
//    const char *delims = ";";
//    char *index = strtok(test_case, delims);
//    char *dbname = strtok(NULL, delims);
//    char *result = strtok(NULL, delims);
//    char *sql = strtok(NULL, delims);
    char *index = strdup(argv[1]);
    char *dbname = strdup(argv[2]);
    char *result = strdup(argv[3]);
    char *sql = strdup(argv[4]);

    printf(dbname);

    return 0;
}
