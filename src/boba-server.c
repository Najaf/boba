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

  // struct to hold message to/from client
  Message message;
  initialize_message(&message);

  char response[BUFFER_SIZE];

  
  /**
   * This bit here is particularly pants, since accept blocks until it has an inbound connection
   *
   * Lookup select, epoll, kqueue, livevent etc to make this better
   */
  while (1) {
    accept_socket = accept_connection_on_socket(boba_server_socket);

    printf("Accepting inbound TCP connection\n");

    while (1) {
      //Get a message, print it out
      if (recv_message(accept_socket, &message) < 1)
        break;
      //printf("Read data (%d bytes): %s\n", message.length, message.content);

      //if the command is ping, send back pong
      if (strncmp(message.content, "ping", 4) == 0) {
        strcpy(response, "pong");
        set_message(&message, response, sizeof(char) * strlen(response));
        send_message(accept_socket, &message);
        continue;
      }

      //if the command is echo, send back everything the user sent us
      if (strncmp(message.content, "echo ", 5) == 0) {
        strcpy(response, message.content + (5 * sizeof(char)));
        set_message(&message, response, sizeof(char) * strlen(response));
        send_message(accept_socket, &message);
        continue;
      }


      // if we get here, the command is unrecognized
      strcpy(response, "unrecognized command");
      set_message(&message, response, sizeof(char) * strlen(response));
      send_message(accept_socket, &message);
    }

    printf("Closing inbound TCP connection\n");

    close_tcp_socket(accept_socket);
  }

  free_message(&message);
}
