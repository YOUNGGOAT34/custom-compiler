#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

#define TABLE_SIZE 20

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

Table *create_table();
void hashmap_insert(HashMap *map, const char *key, Variable *value);
Variable *hashmap_get(HashMap *map, const char *key);

#endif
