restsh: restsh.c command_prompt.c command_prompt.h
	gcc -Wall `pkg-config --libs --cflags libedit libcurl` -g restsh.c command_prompt.c -o restsh

run: restsh
	./restsh

val: restsh
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./restsh
