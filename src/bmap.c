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

int map_set(Map *map, char *key, char *value)
{
  int i = map_hash_key(key);
  // check if value exists at that index
  if (map->items[i]) {
    //it exists
    MapItem *item = map->items[i];

    //if the keys are the same, set the value and return
    if (strcmp(item->key, key) == 0) {
      item->value = value;
      return 0;
    } 

    //the keys aren't the same, so loop forward through 'next' till we get
    //to the end of this list (or hit the same key, in which case set and continue)
    while (item->next) { 
      item = item->next;
      if (strcmp(item->key, key) == 0) {
        item->value = value;
        return 0;
      }
    }
    MapItem *next_item = malloc(sizeof(MapItem));
    next_item->key = key;
    next_item->value = value;
    next_item->next = NULL;

    item->next = next_item;
  } else {
    //it doesn't exist, so allocate and create it
    MapItem *item = malloc(sizeof(MapItem));
    map->items[i] = item;
    item->key = key;
    item->value = value;
    item->next = NULL;
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
  free(item);
  return 0;
}

int map_free(Map *map)
{
  //iterate through all values, freeing anything we find
  int i;
  MapItem *item, *prev;
  for (i = 0; i < map->slots; i++) {
    if ((item = map->items[i])) {
      //if it's just the one, free it
      if (!item->next) {
        free(item);
      } else {
        // loop throguh, freeing as we go
        while(item) {
          prev = item;
          item = item->next;
          free(prev);
        }
      }
    }
  }

  //free the array of pointers
  free(map->items);
  return 0;
}
