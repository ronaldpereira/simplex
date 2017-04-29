all:
	gcc -Wall -Wextra -g simplex.c -o simplex.exe

run:
	./simplex.exe < ../test/input.txt > ../test/output.txt

clean:
	rm -rf simplex

test:
	make all && make run && make clean

# gdb:
#	start < ../test/input.txt > ../test/output.txt
