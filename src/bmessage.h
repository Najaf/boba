#ifndef BMESSAGE_H
#define BMESSAGE_H

#include <stdint.h>

// A message sent over a TCP connection

typedef struct Message_ {
  uint32_t length;
  char *content;
} Message;

void initialize_message(Message *message);
void set_message(Message *message, char *content, uint32_t length);
void free_message(Message *message);
int  recv_message(int fd, Message *message);
int  send_message(int fd, Message *message);

#endif
