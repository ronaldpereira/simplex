#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dimensionCounter(FILE *input, int *lines, int *columns)
{
    char c;
    int flag = 0;
    *lines = -1;
    *columns = 1;

    while(fscanf(input, "%c", &c) != EOF)
    {
        if(c == '{')
            (*lines)++;

        if(*lines == 1 && flag == 0)
        {
            while(1)
            {
                fscanf(input, "%c", &c);

                if(c == '}')
                {
                    flag = 1;
                    break;
                }

                if(c == ',')
                    (*columns)++;

            }
        }
    }
}

int main(int argc, char* argv[])
{
    FILE *input, *output;
    int lines, columns;

    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");

    dimensionCounter(input, &lines, &columns);
    printf("%d X %d\n", lines, columns);

    fclose(input);
    fclose(output);

    return 0;
}
