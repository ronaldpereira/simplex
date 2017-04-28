all:
	gcc -Wall -Wextra -g simplex.c -o simplex

run:
	./simplex ../test/input.txt ../test/output.txt

clean:
	rm -rf simplex

test:
	make all && make run && make clean

valgrind:
	make all && valgrind ./simplex ../test/input.txt ../test/output.txt
