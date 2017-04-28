#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void inputReader(char **stream)
{
    char token;
    int i, j = 1;

    *stream = (char*) malloc(sizeof(char));

    printf("Insert the matrix input (including the primal or dual mode):\n> ");

    for(i = 0; ; i++, j++)
    {
        scanf("%c", &token);
        if(token == '\n')
            break;

        (*stream)[i] = token;
        *stream = (char*) realloc(*stream, (j+1)*sizeof(char));
    }
    (*stream)[i] = '\0';
}

bool detectPrimalDual(char **input)
{
    bool mode; // 0 is Primal, 1 is Dual

    if((*input)[0] == 'P' || (*input)[0] == 'p')
        mode = 0;

    else if((*input)[0] == 'D' || (*input)[0] == 'd')
        mode = 1;

    return mode;
}

void dimensionCounter(char *input, int *lines, int *columns)
{
    char c;
    int flag = 0, i = 0;
    *lines = -1;
    *columns = 1;

    while(input[i] != '\0')
    {
        c = input[i];
        i++;

        if(c == '{')
            (*lines)++;

        if(*lines == 1 && flag == 0)
        {
            while(1)
            {
                c = input[i];
                i++;

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

double **matrixBuilder(char *input, double **matrix)
{
    int i = 0, j = 0, k = 0, l = 0;
    bool neg = 0;
    char c, num[50];

    while(1)
    {
        c = input[l];
        l++;

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

            c = input[l];
            l++;

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

int main()
{
    char *input;
    int lines, columns;
    double **matrix;
    int mode;

    printf("Welcome to C-Implex (my implementation of Simplex algorithm using Bland's Law)\n\nAuthor: Ronald Davi Rodrigues Pereira\nBS student of Computer Science in Federal University of Minas Gerais\n\nOption Menu:\n\t1 - Apply the Simplex algorithm (using Bland's Law) and outputs the optimized solution or a certificate of illimitability or inviability\n\t2 - Given a viable and limited LP, it consults the user to use the primal or dual C-Implex implementation and outputs the solution\n\nInsert a mode > ");

    scanf("%d", &mode);
    printf("%d\n", mode);
    getc(stdin);

    inputReader(&input);

    if(mode == 2)
    {
        mode = detectPrimalDual(&input);
        if(mode == 0)
            printf("Solving mode: Primal\n");
        else if(mode == 1)
            printf("Solving mode: Dual\n");
    }

    dimensionCounter(input, &lines, &columns); // Function to count the input matrix dimension

    matrix = matrixAllocation(lines, columns); // Function to allocate the matrix

    matrixBuilder(input, matrix); // Function to build the matrix from the input file

    printMatrix(matrix, lines, columns);

    /* First mode implementation */



    /* Second mode implementation */

    matrixDisallocation(matrix, lines); // Function to free the allocated space for the matrix

    return 0;
}
