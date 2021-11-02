flags = -Werror

program : main.o
	gcc $(flags) -o program main.o
main.o : main.c
	gcc $(flags) -c main.c

.PHONY clean:
clean :
	rm main.o program
