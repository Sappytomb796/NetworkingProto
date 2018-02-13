#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

struct Packet{
  int error;
  int ack;
  char * message;
};

int serialize(struct Packet incoming, char ** returning);
int deserialize();
#endif
