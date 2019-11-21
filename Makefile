CC=gcc
CFLAGS=-c -Wall

all: p1_1 p1_2 p1_3 p2_4 p2_5 p2_6 p2_7 p3_8 p3_9 p3_10

p1_1: p1_1_client p1_1_server

p1_2: p1_2_client p1_2_server

p1_3: p1_3_client p1_3_server

p2_4: p2_4.o
	$(CC) p2_4.o -o p2_4 -lpthread -lrt

p2_5: p2_5.o
	$(CC) p2_5.o -o p2_5 -lpthread -lrt

p2_6: p2_6.o
	$(CC) p2_6.o -o p2_6 -lpthread -lrt

p2_7: p2_7.o
	$(CC) p2_7.o -o p2_7 -lpthread -lrt
	
p3_8: p3_8_client p3_8_server

p3_9: p3_9_client p3_9_server

p3_10: p3_10.o
	$(CC) p3_10.o -o p3_10

server.o: server.h info.h
	$(CC) $(CFLAGS) server.c

client.o: client.h info.h
	$(CC) $(CFLAGS) client.c

p1_1_server: server.o p1_1_server.o
	$(CC) server.o p1_1_server.o -o p1_1_server -lsocket
	
p1_1_client: client.o server.o p1_1_client.o
	$(CC) client.o server.o p1_1_client.o -o p1_1_client -lsocket

p1_1_server.o: server.h info.h
	$(CC) $(CFLAGS) p1_1_server.c

p1_1_client.o: client.h info.h
	$(CC) $(CFLAGS) p1_1_client.c

p1_2_server: server.o p1_2_server.o
	$(CC) server.o p1_2_server.o -o p1_2_server -lsocket
	
p1_2_client: client.o server.o p1_2_client.o
	$(CC) client.o server.o p1_2_client.o -o p1_2_client -lsocket

p1_2_server.o: server.h info.h
	$(CC) $(CFLAGS) p1_2_server.c

p1_2_client.o: client.h info.h
	$(CC) $(CFLAGS) p1_2_client.c

p1_3_server: server.o p1_3_server.o
	$(CC) server.o p1_3_server.o -o p1_3_server -lsocket
	
p1_3_client: client.o server.o p1_3_client.o
	$(CC) client.o server.o p1_3_client.o -o p1_3_client -lsocket

p1_3_server.o: server.h info.h
	$(CC) $(CFLAGS) p1_3_server.c

p1_3_client.o: client.h info.h
	$(CC) $(CFLAGS) p1_3_client.c

p2_4.o:
	$(CC) $(CFLAGS) p2_4.c

p2_5.o:
	$(CC) $(CFLAGS) p2_5.c

p2_6.o:
	$(CC) $(CFLAGS) p2_6.c

p2_7.o:
	$(CC) $(CFLAGS) p2_7.c
	
p3_8_server: server.o p3_8_server.o
	$(CC) server.o p3_8_server.o -o p3_8_server -lsocket
	
p3_8_client: client.o server.o p3_8_client.o
	$(CC) client.o server.o p3_8_client.o -o p3_8_client -lsocket

p3_8_server.o: server.h info.h
	$(CC) $(CFLAGS) p3_8_server.c

p3_8_client.o: client.h info.h
	$(CC) $(CFLAGS) p3_8_client.c
	
p3_9_server: server.o p3_9_server.o
	$(CC) server.o p3_9_server.o -o p3_9_server -lsocket
	
p3_9_client: client.o server.o p3_9_client.o
	$(CC) client.o server.o p3_9_client.o -o p3_9_client -lsocket

p3_9_server.o: server.h info.h
	$(CC) $(CFLAGS) p3_9_server.c

p3_9_client.o: client.h info.h
	$(CC) $(CFLAGS) p3_9_client.c

p3_10.o:
	$(CC) $(CFLAGS) p3_10.c

clean:
	-rm *.o
