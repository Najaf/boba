#include "bmessage.h"
#include "bnet.h"
#include <string.h>
#include <stdlib.h>

void prepare_message_content_memory(Message *message)
{
  // allocate and clear some memory for the content, +1 for the null byte
  // message->length needs to be set before we get here
  message->content = realloc(message->content, message->length + 1);
  memset(message->content, 0, message->length + 1);
}

int recv_message(int fd, Message *message)
{
  message->length = 0;

  int retval;

  // read 4 bytes into message length
  if ((retval = recv_bytes_on_tcp_socket(fd, (void *)&message->length, sizeof(message->length))) < 1)
    return retval;

  prepare_message_content_memory(message);

  // recv the message into the memory we've allocated for it
  // since we cleared out message->length + 1 bytes, the string is null terminated
  retval = recv_bytes_on_tcp_socket(fd, (void *)message->content, message->length);
  return retval;

}

void free_message(Message *message)
{
  free(message->content);
}

void initialize_message(Message *message)
{
  message->length = 0;
  message->content = NULL;
}

int send_message(int fd, Message *message)
{
  int retval;

  //send the header showing the message length
  if ((retval = send_bytes_on_tcp_socket(fd, (void *)&message->length, sizeof(message->length))) < 1)
    return retval;

  //send the message contents
  retval = send_bytes_on_tcp_socket(fd, (void *)message->content, message->length);
  return retval;
}

void set_message(Message *message, char *content, uint32_t length)
{
  message->length = length;
  prepare_message_content_memory(message);

  memcpy(message->content, content, message->length);
}
