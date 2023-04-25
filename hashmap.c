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

HashMap * createMap(long capacity){
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map -> capacity = capacity;
  map -> current = -1;

  map->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
  
  return map;
}

void insertMap(HashMap * map, void * key, void * value){
  int pos = hash(key, map -> capacity);

  
  while(map -> buckets[pos] != NULL && strcmp(map->buckets[pos] -> key, key) != 0){
    pos = ((pos + 1) % map -> capacity);
  }
  if(map -> buckets[pos] == NULL){
    Pair * Valor = createPair(key, value);
    map -> buckets[pos] = Valor;
    map -> size++;
  }
  else{
    map -> buckets[pos] -> key = key;
    map -> buckets[pos] -> value = value;
  }
  map -> current = pos;
}

void eraseMap(HashMap * map, char * key){
  int pos = hash(key, map -> capacity);
  int inicio = pos;

  while(map -> buckets[pos] != NULL){
    if(strcmp(map -> buckets[pos] -> key, key) == 0){
      map -> buckets[pos] -> key = NULL;
      map -> size--;
      return;
    }
     pos = ((pos + 1) % map -> capacity);
    if(inicio == pos){
      break;
    }
  }
}

/*Pair * searchMap(HashMap * map,  char * key) {   
  int i = hash( key, map->capacity);
  
  while(is_equal(key, map->buckets[i])!=0){
    i = ((i + 1) % map -> capacity);
    if(is_equal(key, map->buckets[i])==1) {
      map->current= i;
      return map-> buckets[i];
    }
    }
  return NULL;
}*/

Pair * searchMap(HashMap * map, void * key){
  int pos = hash(key, map -> capacity);
  int inicio = pos;
  
  while(map -> buckets[pos] != NULL && strcmp(map->buckets[pos] -> key, key) != 0){
    pos = ((pos + 1) % map -> capacity);
    if(inicio == pos){
      return NULL;
    }
  }
  map -> current = pos;
  return map -> buckets[pos];
};

Pair * firstMap(HashMap * map){
  int i;
  for(i=0; i<map->capacity; i++){
    if(map -> buckets[i] != NULL){
      map -> current = i;
      return map -> buckets[i];
    }
  }
    return NULL;
};

Pair * nextMap(HashMap * map);

void enlarge(HashMap * map){
  int old_capacity = map -> capacity;
  map -> capacity *= 2;

  Pair **old_buckets = map -> buckets;

  map -> buckets = (Pair **) calloc(map -> capacity,sizeof(Pair*));
  map -> size = 0;
  for(int pos = 0 ; pos < old_capacity ; pos++){
    if(old_buckets[pos] != NULL){
      insertMap(map,old_buckets[pos] -> key,old_buckets[pos] -> value);
    }
  }
  enlarge_called = 1; //no borrar (testing purposes)
}