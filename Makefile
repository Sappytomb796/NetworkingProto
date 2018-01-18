all: client server

client: 
	gcc -o cs494rcp_client client.c

server:
	gcc -o cs494rcp_server server.c


clean:
	rm -f *~ cs494rcp_client cs494rcp_server
