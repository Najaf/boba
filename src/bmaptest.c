#include "bmap.h"
#include <stdio.h>
#include <string.h>

void print_map_item(MapItem *item)
{
  printf("%s:%s", item->key, item->value);

  if (item->next) {
    printf(" => ");
    print_map_item(item->next);
  } else {
    printf("\n");
  }
}

void print_map(Map *map)
{
  int i;
  for (i = 0; i < map->slots; i++) {
    if (map->items[i]) {
      printf("%d: ", i);
      print_map_item(map->items[i]);
    }   
  }
}

int main()
{
  //printf("Some random hashes:\n");
  //printf("\tBazinga: %d\n", map_hash_key("Bazinga"));
  //printf("\tFlabagoobie: %d\n", map_hash_key("Flabagoobie"));
  //printf("\tScooby: %d\n", map_hash_key("Scooby"));
  //printf("\tMcDooby: %d\n", map_hash_key("McDooby"));
  //printf("\tagnizaB: %d\n", map_hash_key("agnizaB"));

  Map map;
  map_init(&map);

  map_set(&map, "bazinga", "bazonga");
  map_set(&map, "zabinga", "zabonga");
  map_set(&map, "zabniga", "maponya");
  map_set(&map, "kabanooga", "daboogawoofa");

  MapItem *item;

  print_map(&map);
  map_get(&map, "abzinga");

  printf("================================\n");

  print_map(&map);
  map_free(&map);
}
