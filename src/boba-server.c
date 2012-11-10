#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX_CONNECTIONS 128
#define BUFFER_SIZE 1024

const char boba_server_usage_string[] = "boba-server port";

void die(char *message)
{
  printf("Error: %s\n", message);
  exit(1);
}

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
  if ((boba_server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    die("Unable to create socket");

  // bind it to a port
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo("0.0.0.0", port, &hints, &res);

  bind(boba_server_socket, res->ai_addr, res->ai_addrlen);
  
  // listen on the port for connections
  if (listen(boba_server_socket, MAX_CONNECTIONS) == -1)
    die("Unable to open socket");

  printf("Listening for connections on port %s...\n", port);

  int accept_socket;
  struct sockaddr_storage remote_address;
  socklen_t address_size;
  void *buffer;
  int read_bytes;

  address_size = sizeof(remote_address);
  buffer = malloc(BUFFER_SIZE);

  uint32_t message_size = 0;
  char *message = malloc(BUFFER_SIZE * sizeof(char));
  memset(message, 0, sizeof(char) * BUFFER_SIZE);
  
  /**
   * This bit here is particularly pants, since accept blocks until it has an inbound connection
   *
   * Lookup select(1) to make this better
   */
  while (1) {
    if ((accept_socket = accept(boba_server_socket, (struct sockaddr *)&remote_address, &address_size)) == -1)
      die("Unable to accept connection");

    printf("Accepting inbound TCP connection\n");

    while ((read_bytes = recv(accept_socket, buffer, BUFFER_SIZE, 0)) > 0) {
      memcpy(&message_size, buffer, sizeof(uint32_t));
      memcpy(message, (char *)(buffer + sizeof(uint32_t)), message_size);

      printf("Read data (%d bytes): %s\n", message_size, message);
      
      //now that we're done with the message, clear it out
      memset(message, 0, sizeof(char) * BUFFER_SIZE);
    }

    printf("Closing inbound TCP connection\n");

    close(accept_socket);
  }
}
