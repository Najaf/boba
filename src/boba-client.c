#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "bnet.h"

#define BUFFER_SIZE 1024
#define PROMPT_STRING "> "

const char boba_client_usage_string[] = "boba-client host port";

void die(char *message)
{
  printf("Error: %s\n", message);
  exit(1);
}

void prompt()
{
  printf("%s", PROMPT_STRING);
}


int main(int argc, char *argv[])
{
  // get the port number out of the args
  char *host, *port;
  if (argc != 3) {
    printf("Usage: %s\n", boba_client_usage_string);
    exit(1);
  }

  host = argv[1];
  port = argv[2];

  //make a socket and connect to that port
  int boba_client_socket = create_client_socket(host, port);
  printf("Connected to %s on port %s...\n", host, port);

  // now that we're connected, listen for user commands and send them to the server
  char *line = malloc(sizeof(char) * BUFFER_SIZE);
  size_t size = sizeof(char) * BUFFER_SIZE;
  int retval;

  uint32_t message_size;
  int write_length;
  void *send_buffer = malloc((sizeof(uint32_t)) + (sizeof(char) * BUFFER_SIZE));

  // initial prompt
  prompt();

  while((retval = getline(&line, &size, stdin)) != -1) {
    // exit if user punches in 'exit'
    if (strcmp(line, "exit\n") == 0) {
      close(boba_client_socket);
      exit(0);
    }

    message_size = strlen(line) - 1;
    memcpy(send_buffer, (void *)&message_size, sizeof(uint32_t));
    memcpy(send_buffer + sizeof(uint32_t), line, message_size);

    send_bytes_on_tcp_socket(boba_client_socket, send_buffer, sizeof(uint32_t) + strlen(line) -1);

    //send(boba_client_socket, send_buffer, sizeof(uint32_t) + strlen(line) - 1, 0);

    prompt();
  }
}
