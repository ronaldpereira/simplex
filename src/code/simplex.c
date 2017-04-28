#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void dimensionCounter(FILE *input, int *lines, int *columns)
{
    char c;
    int flag = 0;
    *lines = -1;
    *columns = 1;

    while(fscanf((input), "%c", &c) != EOF)
    {
        if(c == '{')
            (*lines)++;

        if(*lines == 1 && flag == 0)
        {
            while(1)
            {
                fscanf((input), "%c", &c);

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

double **matrixAllocation(int lines, int columns)
{
    int i;
    double **matrix;

    matrix = (double**) calloc(lines,sizeof(double*));
    for(i = 0; i < lines; i++)
        matrix[i] = (double*) calloc(columns, sizeof(double*));

    return matrix;
}

double **matrixDisallocation(double **matrix, int lines)
{
    int i;

    for(i = 0; i < lines; i++)
        free(matrix[i]);
    free(matrix);

    return NULL;
}

double **matrixBuilder(FILE *input, double **matrix)
{
    int i = 0, j = 0, k = 0;
    bool neg = 0;
    char c, num[50];

    fseek(input, 0, SEEK_SET); // Get back to the beggining of the file

    while(1)
    {
        fscanf(input, "%c", &c);

        if(((int)c >= 48 && (int) c <= 57) || c == '.') // C is a number  or a dot
        {
            num[k] = c;
            k++;
            num[k] = '\0';
        }

        else if(c == '-')
            neg = 1;

        else if(c == ',')
        {
            matrix[i][j] = atof(num);
            if(neg)
            {
                matrix[i][j] *= -1;
                neg = 0;
            }
            k = 0;
            j++;
        }

        else if(c == '}')
        {
            matrix[i][j] = atof(num);
            if(neg)
            {
                matrix[i][j] *= -1;
                neg = 0;
            }
            k = 0;
            i++;
            j = 0;

            fscanf(input, "%c", &c);
            if(c == '}') // Fim do arquivo de leitura
                break;
        }
    }

    return matrix;
}

void printMatrix(double **matrix, int lines, int columns)
{
    int i, j;

    printf("%d x %d\n", lines, columns);

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

    dimensionCounter(input, &lines, &columns); // Function to count the input matrix dimension

    matrix = matrixAllocation(lines, columns); // Function to allocate the matrix

    matrixBuilder(input, matrix); // Function to build the matrix from the input file

    printMatrix(matrix, lines, columns);

    matrixDisallocation(matrix, lines); // Function to free the allocated space for the matrix
    fclose(input);
    fclose(output);

    return 0;
}
