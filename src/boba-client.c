#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

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
  int boba_client_socket;
  if ((boba_client_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    die("Unable to create socket");

  // connect to a remote host/post
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(host, port, &hints, &res);

  int connection_status;
  if ((connection_status = connect(boba_client_socket, res->ai_addr, res->ai_addrlen)) != 0)
    die("Couldn't connect to given host/port");

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
    memcpy(send_buffer + sizeof(uint32_t), line, strlen(line) - 1);

    send(boba_client_socket, send_buffer, sizeof(uint32_t) + strlen(line) - 1, 0);

    //printf("(%d bytes): %s", retval, line);
    //printf("%d:%lu:%c", retval, sizeof(char), line[(retval / sizeof(char)) - 2]);
    prompt();
  }
}
