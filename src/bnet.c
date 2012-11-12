#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "berror.h"

#define BNET_SERVER 1
#define BNET_CLIENT 2

#define BNET_SEND 1
#define BNET_RECV 2

#define TCP_BACKLOG 128

int create_socket(char *host, char *port, int type)
{
  // try creating the socket
  int boba_socket;
  if ((boba_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    die("Unable to create socket");

  // connect to a remote host/post
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(host, port, &hints, &res);

  if (type == BNET_SERVER) {
    bind(boba_socket, res->ai_addr, res->ai_addrlen);
  }

  int status;
  if (type == BNET_CLIENT) {
    if ((status = connect(boba_socket, res->ai_addr, res->ai_addrlen)) != 0)
      die("Couldn't connect to given host/port");
  } else if(type == BNET_SERVER) {
    if ((status = listen(boba_socket, TCP_BACKLOG)) != 0)
      die("Couldn't connect to given host/port");
  }

  freeaddrinfo(res);

  return boba_socket;
}

int create_server_socket(char *host, char *port)
{
  return create_socket(host, port, BNET_SERVER);
}

int create_client_socket(char *host, char *port)
{
  return create_socket(host, port, BNET_CLIENT);
}

int accept_connection_on_socket(int fd)
{
  int accept_socket;
  struct sockaddr_storage remote_address;
  socklen_t address_size;

  address_size = sizeof(remote_address);

  if ((accept_socket = accept(fd, (struct sockaddr *)&remote_address, &address_size)) == -1)
    die("Unable to accept connection");

  return accept_socket;
}

int transmit_bytes(int fd, void *buffer, int bytes, int type)
{
  int transmitted, total_transmitted;
  transmitted = 0;
  total_transmitted = 0;

  while (total_transmitted < bytes) {
    if (type == BNET_SEND) {
      transmitted = send(fd, buffer + total_transmitted, bytes - total_transmitted, 0);
    } else if(type == BNET_RECV) {
      transmitted = recv(fd, buffer + total_transmitted, bytes - total_transmitted, 0);
    }
    
    if (transmitted < 1) {
      return transmitted;
    }
    total_transmitted += transmitted;
  }
  return total_transmitted;
}

int send_bytes_on_tcp_socket(int fd, void *buffer, int bytes)
{
  return transmit_bytes(fd, buffer, bytes, BNET_SEND);
}

int recv_bytes_on_tcp_socket(int fd, void *buffer, int bytes)
{
  return transmit_bytes(fd, buffer, bytes, BNET_RECV);
}


void close_tcp_socket(int fd)
{
  close(fd);
}
