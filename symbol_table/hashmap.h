#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>


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
typedef enum {
    INTEGER,
    LONG,
    LONGLONG,
    CHARACTER,
    SHORT
    
  }Returntype;

typedef struct {
    char *name;
    Returntype type;
} Param;

typedef struct Function{
    char *name;
    Returntype return_type;
    size_t line_number;
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
 aaditionally I will need the offset since I need to take care of scopes when I will be generation
 assembly ,,this will help to identify where the variable starts and ends on the stack,,like an offset from the base pointer
*/


typedef struct {
    char *name;
    Returntype type;
    int offset;
    bool is_global;
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
    int current_offset;
} Table;



//variable functions
Table *create_table(void);

void print_codegen_variables(void);
unsigned int hash(const char *key); 
void hashmap_insert(HashMap *map, const char *key, Variable *value);
Variable *hashmap_get(HashMap *map, const char *key);
void clear_hashmap(HashMap *map) ;
void table_insert_variable(Table *table, const char *name, Returntype type, size_t line_number, size_t size_of_type,bool);


//function functions
void insert_function(FunctionTable *table, const char *name, Returntype type, Param *params, int param_count, int line_number);
Function *get_function(FunctionTable *table, const char *name);
FunctionTable *create_function_table(void);
void function_table_insert(FunctionTable *table, const char *key, Function *value);
Function *function_table_get(FunctionTable *table, const char *key);
void clear_function_table(FunctionTable *table);

#include "../scope/stack.h"
#endif
