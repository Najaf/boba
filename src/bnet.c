#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die_net(char *message)
{
  printf("Error: %s\n", message);
  exit(1);
}

int create_server_socket(char *host, char *port)
{
  return 0;
}

int create_client_socket(char *host, char *port)
{
  // try creating the socket
  int boba_client_socket;
  if ((boba_client_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    die_net("Unable to create socket");

  // connect to a remote host/post
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(host, port, &hints, &res);

  int connection_status;
  if ((connection_status = connect(boba_client_socket, res->ai_addr, res->ai_addrlen)) != 0)
    die_net("Couldn't connect to given host/port");

  return boba_client_socket;
}

void send_bytes_on_tcp_socket(int fd, void *buffer, int bytes)
{
  int sent, total_sent;
  sent = 0;
  total_sent = 0;

  while (total_sent < bytes) {
    sent = send(fd, buffer + total_sent, bytes - total_sent, 0);
    total_sent += sent;
  }
}

int recv_bytes_on_tcp_socket(int fd, void *buffer, int bytes)
{
  return 0;
}

int close_tcp_socket(int fd)
{
  return 0;
}
