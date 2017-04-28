all:
	gcc -Wall -Wextra -g simplex.c -o simplex.exe

run:
	./simplex.exe ../test/input.txt ../test/output.txt

clean:
	rm -rf simplex

test:
	make all && make run && make clean

valgrind:
	make all && valgrind ./simplex.exe ../test/input.txt ../test/output.txt

gdb:
	start ../test/input.txt ../test/output.txt
