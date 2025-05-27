#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

#define TABLE_SIZE 50
#define FUNCTION_TABLE_SIZE 50

//For functions....
/*
  We need to store:
    function name
    return type
    number of parameters
    parameter names
    parameter return types
    line number that it was defined
    


*/


typedef struct {
    char *name;
    char *type;
} Param;

typedef struct Function{
    char *name;
    char *return_type;
    int line_number;
    Param *params;
    int param_count;
    struct Function *next;
} Function;

typedef struct {
    Function *functions[FUNCTION_TABLE_SIZE];
} FunctionTable;



//for variables
/*
 We need a variable name
 variable type
 and the line number that it was defined.
*/
typedef struct {
    char *name;
    char *type;
    size_t line_number;
} Variable;

typedef struct Entry {
    char *key;
    Variable *value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[TABLE_SIZE];
} HashMap;

typedef struct {
    HashMap *map;
} Table;

//variable functions
Table *create_table(void);
void hashmap_insert(HashMap *map, const char *key, Variable *value);
Variable *hashmap_get(HashMap *map, const char *key);
void clear_hashmap(HashMap *map) ;


//function functions
void insert_function(FunctionTable *table, const char *name, const char *return_type, Param *params, int param_count, int line_number);
FunctionTable *create_function_table(void);
void function_table_insert(Function *table, const char *key, Function *value);
Function *function_table_get(Function *table, const char *key);
void clear_function_table(FunctionTable *table);
#endif
