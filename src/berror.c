#include <stdio.h>
#include <stdlib.h>
#include "berror.h"

void die(char *message)
{
  printf("Error: %s\n", message);
  exit(1);
}
