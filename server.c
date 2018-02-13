#include "header.h"

#define Q_LIMIT 5
#define SIZE    256

int error(int error_code);

int main(int argc, char ** argv)
{
  printf("Server started successfully\n\n");
  struct sockaddr_in server_addr, client_addr;
  int  portnum, clilen, n;
  int  sockfd, newsockfd;
  struct timeval timeout;
  socklen_t clientlen;
  char buf[SIZE];
  char filename[SIZE];
  struct Packet * pack = NULL;
  struct Packet sending;
  char * toSend = NULL;
  FILE *f;
  char * message = "ACK";
  
  pack = (struct Packet *)malloc(sizeof(struct Packet));
  //initialize timeout
  timeout.tv_sec  = 0;
  timeout.tv_usec = 0;

  //Error; boot out-able
  if(argc==1) return error(0);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) return error(1);
  
  bzero((char *) &server_addr, sizeof(server_addr));
  portnum = atoi(argv[1]);  //Check is this supposed to be 1 or 2?
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port= htons(portnum);
  //Bind the socket and the port together
  if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    return error(2);
  
  listen(sockfd, 5);
  clientlen = sizeof(client_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clientlen);
  if(newsockfd < 0) return error(3);
  
  //3 way handshake;
  //read from the socket
  bzero(buf, SIZE);
  //timeout if nothing received
  //while(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, 
  //		 (char *)&timeout, sizeof(timeout)) != 0) 
  n = read(newsockfd, buf, SIZE);
  if (n < 0) return error(4);
  deserialize(buf, &pack);
  
  sending.error = 1;
  sending.ack   = 1;
  strcpy(sending.message, message);
  serialize(sending, &toSend);  
  printf("toSend: %s", toSend);


  if(pack->ack == 1 && pack->error == 1)
    n = write(newsockfd, toSend, sizeof(toSend));
  if (n < 0) return error(5);

  printf("Message 1: %s\n", pack->message);











  //free(pack);
  pack = NULL;
  //The file should not be modified!! It should be protected.
  //f = fopen(filename, "r");

  //This is where we will expect the request for a file.
  bzero(buf, SIZE);
  n = read(newsockfd, buf, SIZE);
  if (n < 0) return error(4);
  
  //printf("buff:");
  //printf("%s", buf);
  //deserialize(buf, &pack);
  
  //printf("filename: %s", pack->message);

  close(newsockfd);
  close(sockfd);
  printf("\nServer exiting\n");

  return 0;
}


//Send in the error code for a customized error message, or -1 for default of just error.
int error(int error_code)
{
  switch (error_code){
  case 0:
    printf("ERROR: Did not provide port number\n");
    break;
  case 1:
    printf("ERROR: Error establishing socket file descriptor (sockfd)\n");
    break;
  case 2:
    printf("ERROR: Did not bind\n");
    break;
  case 3:
    printf("ERROR: Did not accept\n");
    break;
  case 4:
    printf("ERROR: Could not read from socket\n");
    break;
  case 5:
    printf("ERROR: Could not write to socket\n");
    break;
  default:
    printf("ERROR\n");
    break;
  }
  printf("\nServer exiting due to error\n");  
  exit(0);
}
