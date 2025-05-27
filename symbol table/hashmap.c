#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + (*key++);
    }
    return hash % TABLE_SIZE;
}



//functions

FunctionTable *create_function_table() {
    FunctionTable *table= malloc(sizeof(FunctionTable));
    if (!table) {
        perror("Failed to allocate FunctionMap");
        exit(1);
    }

    for (int i = 0; i < FUNCTION_TABLE_SIZE; i++) {
        table->functions[i] = NULL;
    }
    return table;
}

void insert_function(FunctionTable *table, const char *name, const char *return_type, Param *params, int param_count, int line_number) {
    unsigned int index = hash(name);
    Function *function = malloc(sizeof(Function));
    function->name = strdup(name);
    function->return_type = strdup(return_type);
    function->param_count = param_count;
    function->line_number = line_number;
    function->params = malloc(sizeof(Param) * param_count);
    if (!function->params) {
        perror("Failed to allocate parameters");
        exit(1);
    }
    for (int i = 0; i < param_count; i++) {
        function->params[i].name = strdup(params[i].name);
        function->params[i].type = strdup(params[i].type);
    }
    function->next = table->functions[index];
    table->functions[index] = function;
}

Function *get_function(FunctionTable *table, const char *name) {
    unsigned int index = hash(name);
    Function *function = table->functions[index];

    while (function != NULL) {
        if (strcmp(function->name, name) == 0) {
            return function;
        }
         function= function->next;
    }

    return NULL;
}

void clear_function_table(FunctionTable *table) {
    for (int i = 0; i < FUNCTION_TABLE_SIZE; i++) {
        Function *function = table->functions[i];
        while (function) {
            Function *tmp =function;
            function = function->next;

            free(tmp->name);
            free(tmp->return_type);
            for (int j = 0; j < tmp->param_count; j++) {
                free(tmp->params[j].name);
                free(tmp->params[j].type);
            }
            free(tmp->params);
            free(tmp);
        }
    }
    free(table);
}




//variables

Table *create_table(void) {
    Table *table = malloc(sizeof(Table));
     if(!table){
        perror("Failed to allocate  table");
        exit(1);
     }
    table->map = malloc(sizeof(HashMap));
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->map->buckets[i] = NULL;
    }
    return table;
}

void hashmap_insert(HashMap *map, const char *key, Variable *value) {
    unsigned int index = hash(key);
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;
}


Variable *hashmap_get(HashMap *map, const char *key) {
    unsigned int index = hash(key);
    Entry *entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

//clear the hashmap variables after getting out of the scope...
void clear_hashmap(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *entry = map->buckets[i];
        while (entry) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp); // Free the Entry
        }
    }
    free(map); 
}
