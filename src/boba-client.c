#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define PROMPT_STRING "> "

void prompt()
{
  printf("%s", PROMPT_STRING);
}


int main()
{
  char *line = malloc(sizeof(char) * BUFFER_SIZE);
  size_t size = sizeof(char) * BUFFER_SIZE;
  int retval;

  // initial prompt
  prompt();

  while((retval = getline(&line, &size, stdin)) != -1) {
    printf("Gonna do something with: %s", line);
    prompt();
  }
}
