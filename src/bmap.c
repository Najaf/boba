/*
Implements a basic chained hash table.

Notes:
  * Implement a better hash function. At the moment it returns sum (the ascii
    values of all the characters in the string) modulo size of the array.
  * Consider whether we really need so many reallocs for the string keys and
    values. Might their be a cleverer way of getting around this? Might setting
    a maximum key length be good enough?
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmap.h"

int map_hash_key(char *key)
{
  int i = 0;
  int sum = 0;
  char c = *key;
  while (c != '\0') {
    sum = sum + c;
    c = *(key + ++i);
  }
  return (sum % HASH_SIZE);
}

void map_init(Map *map)
{
  map->slots = HASH_SIZE;
  map->items = malloc(sizeof(*map->items) * HASH_SIZE);
  memset(map->items, 0, sizeof(*map->items) * HASH_SIZE);
}

void map_item_init(MapItem *item, char *key, char *value, MapItem *next)
{
  size_t key_size, value_size;

  key_size = key ? sizeof(char) * (strlen(key) + 1) : START_KEY_SIZE;
  item->key = (char *)malloc(key_size);
  memset(item->key, 0, key_size);
  if (key) { strcpy(item->key, key); }

  value_size = value ? sizeof(char) * (strlen(value) + 1) : START_VALUE_SIZE;
  item->value = (char *)malloc(value_size);
  memset(item->value, 0, value_size);
  if (value) { strcpy(item->value, value); }
}

void map_item_set(MapItem *item, char *key, char *value)
{
  size_t size;
  if (key) {
    size = sizeof(char) * (strlen(key) + 1);
    item->key = (char *)realloc(item->key, size );
    strcpy(item->key, key);
  }

  if (value) {
    size = sizeof(char) * (strlen(value) + 1);
    item->value = (char *)realloc(item->value, size );
    strcpy(item->value, value);
  }
}

int map_set(Map *map, char *key, char *value)
{
  int i = map_hash_key(key);
  // check if value exists at that index
  if (map->items[i]) {
    //it exists
    MapItem *item = map->items[i];

    //if the keys are the same, set the value and return
    if (strcmp(item->key, key) == 0) {
      map_item_set(item, NULL, value);
      return 0;
    } 

    //the keys aren't the same, so loop forward through 'next' till we get
    //to the end of this list (or hit the same key, in which case set and continue)
    while (item->next) { 
      item = item->next;
      if (strcmp(item->key, key) == 0) {
        map_item_set(item, NULL, value);
        return 0;
      }
    }
    MapItem *next_item = malloc(sizeof(MapItem));
    map_item_init(next_item, key, value, NULL);
    next_item->next = NULL;
    item->next = next_item;
  } else {
    //it doesn't exist, so allocate and create it
    MapItem *item = malloc(sizeof(MapItem));
    map_item_init(item, key, value, NULL);
    item->next = NULL;
    map->items[i] = item;
  }

  return 0;
}

MapItem *map_get(Map *map, char *key)
{
  int i = map_hash_key(key);
  MapItem *item = map->items[i];
  if (!item) {
    return NULL;
  }

  //cycle through the list till the end or a matching key string
  while (item != NULL && strcmp(item->key, key) != 0) {
    item = item->next;
  }

  if (item) {
    return item;
  } else {
    return NULL;
  }
}

/**
 * Delete an item from the map
 *
 *  0 if item found and deleted
 * -1 if item not found
 */
int map_del(Map *map, char *key)
{
  int i = map_hash_key(key);
  MapItem *item = map->items[i];

  // return if ths slot is empty
  if (!item) {
    return -1;
  }

  // We have an item in the first slot

  // If the key to the first one matches, delete it
  // (also handle condition if its next is set)
  if (strcmp(item->key, key) == 0) {
    if (item->next) {
      map->items[i] = item->next;
    } else {
      map->items[i] = NULL;
    }
    map_item_free(item, 0);
    free(item);
    return 0;
  }

  // The key of the first item in slot doesn't match
  // loop through until we find a matching key

  MapItem *previous; //keep track of previous map item
  while (item != NULL && strcmp(item->key, key) != 0) {
    previous = item;
    item = item->next;
  }

  //if we get here and the item is null, it's not in the map
  if (!item) {
    return -1;
  }

  //if item has a next, set previous->next to it
  if (item->next) {
    previous->next = item->next;
  } else {
    previous->next = NULL;
  }

  //free the item and return
  map_item_free(item, 0);
  free(item);
  return 0;
}

void map_item_free(MapItem *item, int free_all)
{
  if (item->key)
    free(item->key);

  if (item->value)
    free(item->value);

  if (item->next && free_all) {
    map_item_free(item->next, 1);
    free(item->next); 
  }
}

int map_free(Map *map)
{
  //iterate through all values, freeing anything we find
  int i;
  MapItem *item, *prev;
  for (i = 0; i < map->slots; i++) {
    if ((item = map->items[i])) {
      map_item_free(item, 1);
      free(item);
    }
  }

  //free the array of pointers
  free(map->items);
  return 0;
}
