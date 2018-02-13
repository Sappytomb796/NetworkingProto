#include "header.h"

#define SIZE 256
void error(int error_code);

//Serialize and Deserialize examples in the lecture before the off day.
//Socket options for timers.

int main(int argc, char **argv)
{
  printf("Client running successfully\n\n");
  struct sockaddr_in server_addr;
  struct hostent *server;
  struct timeval timeout;
  int sockfd, portnum, n;
  char * toSend = NULL;  
  struct Packet pack;
  struct Packet * received = NULL;
  char * filename;
  char buf[SIZE];
  int isOk;

  if(argc < 3) error(0);

  received = (struct Packet *)malloc(sizeof(struct Packet));

  pack.error = 1;
  pack.ack = 1;
  pack.message = NULL;
  pack.message = (char *)malloc(strlen(argv[3]) +1);
  strcpy(pack.message, argv[3]);
  isOk = serialize(pack, &toSend);
  if(isOk != 1) error(20); //Just general error packet failed to form.

  //initialize timeout
  timeout.tv_sec  = 0;
  timeout.tv_usec = 0;
  
  portnum = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) error(1);

  server = gethostbyname(argv[1]);
  if(server == NULL) error(2);

  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, 
	server->h_length);
  server_addr.sin_port = htons(portnum);
  if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) error(3);

  //Start 3 way handshake
  n = write(sockfd, toSend, strlen(toSend));
  if(n < 0) error(4);
  //timeout if nothing received
  //if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, 
  //		 (char *)&timeout, sizeof(timeout)) < 0) error(20);
  
  bzero(buf, SIZE);
  n = read(sockfd, buf, SIZE);
  if(n < 0) error(5);
  printf("buffer: %s\n", buf);
  deserialize(buf, &received);

  received = NULL;
  bzero(buf, SIZE);
  //printf("error stat: %d\nAck stat: %d\n", received->error, received->ack);
  //if(received->error == 1 && received->ack == 1){
  //n = write(sockfd, toSend, strlen(toSend));
  //  if(n < 0) error(4); //}




  close(sockfd);
  printf("\nClient exiting\n");
  return 0;
}

void error(int error_code)
{
  switch (error_code){
  case 0:
    printf("ERROR: Did not provide port number\n");
    break;
  case 1:
    printf("ERROR: Error establishing socket file descriptor (sockfd)\n");
    break;
  case 2:
    printf("ERROR: No host with that name\n");
    break;
  case 3:
    printf("ERROR: Did not accept\n");
    break;
  case 4:
    printf("ERROR: Could not write to socket\n");
    break;
  case 5:
    printf("ERROR: Could not read from socket\n");
    break;
  default:
    printf("ERROR\n");
    break;
  }
  printf("\nServer exiting due to error\n");  
  exit(0);
}
