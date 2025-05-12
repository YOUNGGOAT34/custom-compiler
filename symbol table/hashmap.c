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

Table *create_table() {
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
    free(map); // Free the HashMap itself if you plan to allocate a new one
}
