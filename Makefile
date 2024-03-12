pong: pong.o networking.o
	gcc -o pong pong.o networking.o -lncurses -g

pong.o: pong.c
	gcc -c pong.c -o pong.o -g

networking.o: networking.c
	gcc -c networking.c -o networking.o -g


