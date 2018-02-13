#include "header.h"

int serialize(struct Packet incoming, char ** returning)
{
  char * temp = NULL;

  //  if(*returning != NULL){
  //  free(*returning);
  //  *returning = NULL; }

  *returning = NULL;
  //Allocate room for the message, 2 for the error and
  //  ack, and 1 for the null terminator.
  temp = (char *)malloc(strlen(incoming.message) + 3);
  
  
  //Hard coding the header portion to return success on 
  //  success only, and error or NAK on anything ELSE
  if(incoming.error == 1)
    temp[0] = '1';
  else
    temp[0] = '0';
  if(incoming.ack == 1)
    temp[1] = '1';
  else
    temp[1] = '0';

  strcpy((temp+2), incoming.message);

  *returning = temp;
  return 1;
}


int deserialize(char * packet, struct Packet ** returning)
{
  struct Packet * temp;
  char tempChar[1];
  char * tempString;
  int holder;


  if (!*returning)
    return 0;

  tempChar[0] = packet[0];
  holder = atoi(tempChar);
  temp->error = holder;
  tempChar[0] = packet[1];
  holder = atoi(tempChar);
  temp->ack = holder;

  if(temp->message != NULL){
    temp->message = NULL; }

  tempString = (char *)malloc(strlen(packet) -1);

  strcpy(tempString, (packet)+2);
  
  temp->message = tempString;

  *returning = temp;
  return 1;
}
