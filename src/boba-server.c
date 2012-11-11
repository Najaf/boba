#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bnet.h"
#include "berror.h"
#include "bmessage.h"

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

  // struct to hold our message
  Message message;
  initialize_message(&message);
  
  /**
   * This bit here is particularly pants, since accept blocks until it has an inbound connection
   *
   * Lookup select, epoll, kqueue, livevent etc to make this better
   */
  while (1) {
    accept_socket = accept_connection_on_socket(boba_server_socket);

    printf("Accepting inbound TCP connection\n");

    while (1) {
      if (recv_message(accept_socket, &message) < 1)
        break;
      printf("Read data (%d bytes): %s\n", message.length, message.content);
    }

    printf("Closing inbound TCP connection\n");

    close_tcp_socket(accept_socket);
  }

  free_message(&message);
}
