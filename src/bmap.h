#ifndef BMAP_H
#define BMAP_H

#define HASH_SIZE 1699

typedef struct MapItem_ {
  char *key;
  char *value;
  struct MapItem_ *next;
} MapItem;

typedef struct Map_ {
  MapItem **items;
  int slots;
} Map;

void map_init(Map *map);
int map_set(Map *map, char *key, char *value);
MapItem *map_get(Map *map, char *key);
int map_del(Map *map, char *key);
int map_hash_key(char *key);
int map_free(Map *map);

#endif
