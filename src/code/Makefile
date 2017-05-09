all:
	gcc -Wall -Wextra -g simplex.c -o simplex.exe

run:
	./simplex.exe ../test/input.txt ../test/output.txt

clean:
	rm -rf *.exe

test:
	make all && make run && make clean

test2:
	make all && ./simplex.exe && make clean
