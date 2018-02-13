all: client server

client: client.c header.h header.c
	gcc client.c header.h header.c -o cs494rcp_client 

server: server.c header.h header.c
	gcc server.c header.h header.c -o cs494rcp_server


clean:
	rm -f *~ cs494rcp_client cs494rcp_server
