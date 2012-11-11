#include "bmessage.h"
#include "bnet.h"
#include <string.h>
#include <stdlib.h>

int recv_message(int fd, Message *message)
{
  message->length = 0;

  int retval;

  // read 4 bytes into message length
  if ((retval = recv_bytes_on_tcp_socket(fd, (void *)&message->length, sizeof(message->length))) < 1)
    return retval;

  // allocate and clear some memory for the content, +1 for the null byte
  message->content = realloc(message->content, message->length + 1);
  memset((void *)message->content, 0, message->length + 1);

  // recv the message into the memory we've allocated for it
  // since we cleared out message->length + 1 bytes, the string is null terminated
  retval = recv_bytes_on_tcp_socket(fd, (void *)message->content, message->length);
  return retval;

}

void free_message(Message *message)
{
  if (message->content != NULL) {
    free(message->content);
  }
}

void initialize_message(Message *message)
{
  message->length = 0;
  message->content = NULL;
}

void send_message(int fd, Message *message)
{
}
