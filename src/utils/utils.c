//
// Created by Sam on 2018/1/25.
//

#include <stddef.h>
#include <ctype.h>
#include <malloc.h>
#include <dongmendb/dongmendb.h>
#include <parser/expression.h>
#include "utils/utils.h"

char *new_id_name(){
    char *name = (char *) calloc(MAX_ID_NAME_LENGTH, sizeof(char *));
//    char *name = (char *)malloc(MAX_ID_NAME_LENGTH);
//    memset(name, 0, MAX_ID_NAME_LENGTH);
    return name;
};

int bytes2int(unsigned char x1, unsigned char x2, unsigned char x3, unsigned char x4){
    return (x1 << 24) | (x2 << 16) | (x3 << 8) | x4;
};

int dongmendb_tokenize(char *str, char ***tokens)
{
    char *s;
    int ntokens = 0;

    s = str;
    if (s==NULL)
        return DONGMENDB_ENOMEM;

    /* First pass: Add \0 at the end of each token
     * and count the number of tokens */
    while(isspace(*s)) s++;

    while(*s != '\0')
    {
        ntokens++;
        if (*s == '"')
        {
            s++;
            while(*s && *s != '"') s++;
        }
        else
            while(*s && !isspace(*s)) s++;

        if(*s != '\0')
        {
            *s++ = '\0';
            while(*s && isspace(*s)) s++;
        }
    }


    /* Second pass: Create the array of tokens */
    *tokens = malloc(sizeof(char**) * ntokens);

    s = str;
    while(isspace(*s)) s++;
    for(int i=0; i<ntokens; i++)
    {
        if (*s == '"')
        {
            s++;
            (*tokens)[i] = s;
            while(*s && *s != '"') s++;
        }
        else
        {
            (*tokens)[i] = s;
            while(*s && !isspace(*s)) s++;
        }

        s++;
        while(*s && isspace(*s)) s++;
    }

    return ntokens;
}

