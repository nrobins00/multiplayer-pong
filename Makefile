pong: pong.o
	gcc pong.o -o pong -lncurses

pong.o: pong.c
	gcc -c pong.c -o pong.o



