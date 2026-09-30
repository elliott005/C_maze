CFLAGS=-Wall
FILES_IN=./*.c
FILE_OUT=main.out
ARGS=
all: compile run
run:
	./$(FILE_OUT) $(ARGS)
compile:
	gcc $(CFLAGS) $(FILES_IN) -o $(FILE_OUT)
debug:
	gcc -g $(CFLAGS) $(FILES_IN) -o $(FILE_OUT)
	gdb $(FILE_OUT)
val:
	gcc -g $(CFLAGS) $(FILES_IN) -o $(FILE_OUT)
	valgrind --leak-check=full ./$(FILE_OUT)
clean:
	rm $(FILE_OUT)