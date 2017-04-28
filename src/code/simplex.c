#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dimensionCounter(FILE **input, int *lines, int *columns)
{
    char c;
    int flag = 0;
    *lines = -1;
    *columns = 1;

    while(fscanf((*input), "%c", &c) != EOF)
    {
        if(c == '{')
            (*lines)++;

        if(*lines == 1 && flag == 0)
        {
            while(1)
            {
                fscanf((*input), "%c", &c);

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

double **matrixAllocation(int *lines, int *columns)
{
    int i;
    double **matrix;

    matrix = (double**) calloc(*lines,sizeof(double*));
    for(i = 0; i < *lines; i++)
        matrix[i] = (double*) calloc(*columns, sizeof(double*));

    return matrix;
}

double **matrixDisallocation(double **matrix, int *lines, int *columns)
{
    int i;

    for(i = 0; i < *lines; i++)
        free(matrix[i]);
    free(matrix);

    return NULL;
}

void printMatrix(double **matrix, int lines, int columns)
{
    int i, j;

    for(i = 0; i < lines; i++)
    {
        for(j = 0; j < columns; j++)
        {
            printf("%.5lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    FILE *input, *output;
    int lines, columns;
    double **matrix;

    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");

    dimensionCounter(&input, &lines, &columns);
    //TODO printftest
    printf("%d X %d\n", lines, columns);

    matrix = matrixAllocation(&lines, &columns);

    printMatrix(matrix, lines, columns);

    matrixDisallocation(matrix, &lines, &columns);
    fclose(input);
    fclose(output);

    return 0;
}
