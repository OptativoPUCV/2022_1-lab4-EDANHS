#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void redimensionar(HashMap *map){
    long k;
    Pair **new = (Pair**)calloc(map->capacity*2, sizeof(Pair*));
    for (k = 0 ; k < map->capacity ; k++){
        new[k] = map->buckets[k];
    }
    map->capacity *= 2;
}

void insertMap(HashMap * map, char * key, void * value) {
    long i = hash(key,map->capacity);
    
    if((map->size/map->capacity) >= 0.75) redimensionar(map);

    while (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
        if(is_equal(key,map->buckets[i]->key) == 1) return;
        i = (i+1) % map->capacity;
    }
    if(map->buckets[i] != NULL){
        map->buckets[i]->key = key;
        map->buckets[i]->value = value;
    }
    else map->buckets[i]=createPair(key,value);
    map->size++;
    map->current = i;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap *new = (HashMap*) malloc(sizeof(HashMap));
    if(new == NULL) exit(1);
    new->buckets = (Pair **) calloc(capacity, sizeof(Pair*));
    if(new->buckets == NULL) exit(1);
    new->capacity = capacity;
    new->size = 0;
    new->current = -1;
    return new;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair *erase = searchMap(map,key);
    if(erase == NULL) return;
    erase->key = NULL;
    map->size--;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long i = hash(key,map->capacity);
    while(is_equal(map->buckets[i]->key,key) != 1){
        i = (i+1)%map->capacity;
        if(map->buckets[i] == NULL) return NULL;
    }
    map->current = i;
    return map->buckets[i];
}

Pair * firstMap(HashMap * map) {
    long i = 0;
    while(i < map->capacity){
       if(map->buckets[i] != NULL && map->buckets[i]->key != NULL) break;
       i++;
    }
    map->current = i;
    return map->buckets[i];
}

Pair * nextMap(HashMap * map) {
    long i = map->current;
    while (map->buckets[i] != NULL) i++;
    map->current = i;
    return map->buckets[i];
}
