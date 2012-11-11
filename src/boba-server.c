#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bnet.h"
#include "berror.h"

#define MAX_CONNECTIONS 128
#define BUFFER_SIZE 1024

const char boba_server_usage_string[] = "boba-server port";

/**
 * Takes a single argument for the port number to accept connections on
 */
int main(int argc, char *argv[])
{
  // get the port number to listen to out of the arguments
  char *port;
  if (argc != 2) {
    printf("Usage: %s\n", boba_server_usage_string);
    exit(1);
  }
  port = argv[1];

  // make a socket
  int boba_server_socket;

  boba_server_socket = create_server_socket("0.0.0.0", port);
  printf("Listening for connections on port %s...\n", port);

  int accept_socket;
  void *buffer;
  int read_bytes;

  buffer = malloc(BUFFER_SIZE);

  uint32_t message_size = 0;
  char *message = malloc(BUFFER_SIZE * sizeof(char));
  memset(message, 0, sizeof(char) * BUFFER_SIZE);

  int retval;
  
  /**
   * This bit here is particularly pants, since accept blocks until it has an inbound connection
   *
   * Lookup select, epoll, kqueue, livevent etc to make this better
   */
  while (1) {
    accept_socket = accept_connection_on_socket(boba_server_socket);

    printf("Accepting inbound TCP connection\n");

    while (1) {
      if ((recv_bytes_on_tcp_socket(accept_socket, buffer, sizeof(uint32_t))) < 1)
        break;
      memcpy(&message_size, buffer, sizeof(uint32_t));

      if ((recv_bytes_on_tcp_socket(accept_socket, buffer + sizeof(uint32_t), message_size)) < 1)
        break;
      memcpy(message, (char *)(buffer + sizeof(uint32_t)), message_size);

      printf("Read data (%d bytes): %s\n", message_size, message);
      
      //now that we're done with the message, clear it out
      memset(message, 0, sizeof(char) * BUFFER_SIZE);
    }

    printf("Closing inbound TCP connection\n");

    close_tcp_socket(accept_socket);
  }
}
