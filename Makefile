host: host.o pong.o networking.o
	gcc -o host host.o pong.o networking.o -lncurses -g

client: client.o pong.o networking.o
	gcc -o client client.o pong.o networking.o -lncurses -g

pong: pong.o networking.o
	gcc -o pong pong.o networking.o -lncurses -g

client.o: client.c
	gcc -c client.c -o client.o -g

host.o: host.c
	gcc -c host.c -o host.o -g

pong.o: pong.c
	gcc -c pong.c -o pong.o -g

networking.o: networking.c
	gcc -c networking.c -o networking.o -g


