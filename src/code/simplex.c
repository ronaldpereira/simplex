#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reader()
{
	
}

int main(int argc, char* argv[])
{
    FILE *input, *output;
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");

    fclose(input);
    fclose(output);

    return 0;
}
