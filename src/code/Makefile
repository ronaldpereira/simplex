all:
	gcc -Wall -Wextra -g simplex.c -o simplex.exe

run:
	./simplex.exe

clean:
	rm -rf simplex

test:
	make all && make run && make clean

test2:
	make all && ./simplex.exe && make clean
