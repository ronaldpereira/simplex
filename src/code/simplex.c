#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

double **matrixAllocation(int lines, int columns) // Function to allocate the matrix
{
    int i;
    double **matrix;

    matrix = (double**) calloc(lines,sizeof(double*));
    for(i = 0; i < lines; i++)
        matrix[i] = (double*) calloc(columns, sizeof(double*));

    return matrix;
}

void printLineMatrix(double **matrix, int lines, int columns, FILE* output) // Function to print the matrix in one line
{
    int i, j;

    fprintf(output, "{");
    for(i = 0; i < lines; i++)
    {
        for(j = 0; j < columns; j++)
        {
            if(j == 0)
                fprintf(output, "{");

            if(matrix[i][j] == 0)
                fprintf(output, "0");
            else if((matrix[i][j] - (int)matrix[i][j]) == 0) // If the cell is a integer number, do not print it with decimal part
                fprintf(output, "%.0lf", matrix[i][j]);
            else
                fprintf(output, "%.5lf", matrix[i][j]);


            if(j < columns-1)
                fprintf(output, ",");
            else
                fprintf(output, "}");
        }
        if(i < lines-1)
            fprintf(output, ",");
    }
    fprintf(output, "}\n");
}

bool detectPrimalDual(char input) // Function to detect if, in mode 2, is primal or dual solving mode
{
    bool mode; // 0 is Primal, 1 is Dual

    if(input == 'P')
        mode = 0;

    else if(input == 'D')
        mode = 1;

    return mode;
}

double **matrixBuilder(char *input, double **matrix) // Function that converts the input line matrix to a two-dimension matrix
{
    int i = 0, j = 0, k = 0, l = 0;
    bool neg = 0;
    char c, num[50];

    while(1)
    {
        c = input[l];
        l++;

        if(((int)c >= 48 && (int) c <= 57) || c == '.') // C is a number or a dot
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

            if(c == '}') // End of input file
                break;
        }
    }

    return matrix;
}

double **buildTableau(double **matrix, int *lines, int *columns) // Function that builds the Tableau matrix
{
    int i, j;
    int origcolumns = *columns;
    double **tableau;

    for(i = 0; i < *columns; i++) // Do the -C^t part
    {
        matrix[0][i] *= -1;
    }

    *columns += (2 * (*lines - 1)); // Adds the operations matrix, the identity matrix and b vector on the number of columns

    tableau = matrixAllocation(*lines, *columns);

    for(i = 0; i < *lines; i++)
    {
        for(j = 0; j < *columns; j++)
        {
            if(j < (*lines-1) && j == (i - 1)) // We are on the operations matrix
                tableau[i][j] = 1;

            else if(j >= (*lines-1) && j <= (*columns - (*lines + 1))) // We are on the A matrix
                tableau[i][j] = matrix[i][j-(*lines-1)];

            else if(j >= ((*lines-1)+(origcolumns-1)) && j < (*columns-1) && j + 1 == i + ((*lines-1)+(origcolumns-1))) // We are on the identity matrix
                tableau[i][j] = 1;

            else if(j == (*columns-1)) // We are on the b matrix
                tableau[i][j] = matrix[i][origcolumns-1];
        }
    }

    return tableau;
}

double **buildAuxiliarToTableau(double **matrix, int lines, int columns) // Function that builds the auxiliar matrix in Tableau
{
    int i, j;
    double **auxiliar;

    auxiliar = matrixAllocation(lines, columns);

    for(j = 0; j < columns; j++)
    {
        for(i = 0; i < lines; i++)
        {
            if(j < columns-1 && i != 0)
                auxiliar[i][j] = matrix[i][j];

            else if(j == columns-1)
                auxiliar[i][j] = matrix[i][j];
        }
    }

    for(i = 1; i < lines; i++)
    {
        for(j = 0; j < columns; j++)
            auxiliar[0][j] -= auxiliar[i][j];
    }

    return auxiliar;
}

void unviableCertificate(double **matrix, int lines, FILE *output) // Function that outputs the unviable certificate
{
    int i;

    fprintf(output, "PL inviavel, aqui esta um certificado {");
    for(i = 0; i < lines-1; i++)
    {
        if(matrix[0][i] == 0)
            fprintf(output, "0");
        else if((matrix[0][i] - (int)matrix[0][i]) == 0)
            fprintf(output, "%.0lf", matrix[0][i]);
        else
            fprintf(output, "%.5lf", matrix[0][i]);

        if(i < lines-2)
            fprintf(output, ",");
    }

    fprintf(output, "}\n");
}

void unlimitedCertificate(double **matrix, int lines, int columns, int base, int *bases, FILE *output) // Function that outputs the unlimited certificate
{
    int i, j, k;

    fprintf(output, "PL ilimitada, aqui esta um certificado {");
    for(j = lines-1; j < columns-lines; j++)
    {
        for(k = 0; k < lines-1; k++)
        {
            if(j == bases[k])
            {
                for(i = 1; i < lines; i++)
                {
                    if(matrix[i][j] == 1)
                    {
                        if(matrix[i][base] == 0)
                            fprintf(output, "0");
                        else if((matrix[i][base] - (int)matrix[i][base]) == 0)
                            fprintf(output, "%.0lf", -1*matrix[i][base]);
                        else
                            fprintf(output, "%.5lf", -1*matrix[i][base]);
                    }
                }
                break;
            }
        }

        if(j == base)
            fprintf(output, "1");

        else if(k == lines-1)
            fprintf(output, "0");

        if(j < columns-lines-1)
            fprintf(output, ",");
    }
    fprintf(output, "}\n");
}

void viableSolution(double **matrix, int lines, int columns, int *bases, FILE *output) // Function that outputs the optimum viable solution, the objective value and the dual solution
{
    int i, j, k;

    fprintf(output, "Solucao otima x = {");
    for(j = lines-1; j < columns-lines; j++)
    {
        for(k = 0; k < lines-1; k++)
        {
            if(j == bases[k])
            {
                for(i = 1; i < lines; i++)
                {
                    if(matrix[i][j] == 1)
                    {
                        if((matrix[i][columns-1] - (int)matrix[i][columns-1]) == 0)
                            fprintf(output, "%.0lf", matrix[i][columns-1]);
                        else
                            fprintf(output, "%.5lf", matrix[i][columns-1]);
                    }
                }
                break;
            }
        }
            if(k == lines-1)
                fprintf(output, "0");

            if(j < columns-lines-1)
                fprintf(output, ",");
    }

    fprintf(output, "}, com valor objetivo ");

    if((matrix[0][columns-1] - (int)matrix[0][columns-1]) == 0)
        fprintf(output, "%.0lf", matrix[0][columns-1]);
    else
        fprintf(output, "%.5lf", matrix[0][columns-1]);

    fprintf(output, ", e solucao dual y = {");

    for(i = 0; i < lines-1; i++)
    {
        if((matrix[0][i] - (int)matrix[0][i]) == 0)
            fprintf(output, "%.0lf", matrix[0][i]);
        else
            fprintf(output, "%.5lf", matrix[0][i]);

        if(i < lines-2)
            fprintf(output, ",");
    }
    fprintf(output, "}\n");
}

double **primalTableauSolver(double **matrix, int lines, int columns, int mode, int **bases, FILE *output) // Function that solves the given LP in the primal Tableau algorithm, using Bland's Law
{
    int i, j, base, pivot;
    double minimum, aux, linedivider, multiplier;

    while(1)
    {
        if(mode == 2)
            printLineMatrix(matrix, lines, columns, output);

        base = 0;
        for(i = lines-1; i < columns-1; i++)
        {
            if(matrix[0][i] < -0.000001)
            {
                base = i;
                break;
            }
        }

        if(base == 0) // C^t >= 0
            break;

        else if(base != 0) // Unlimited LP test
        {
            for(i = 1; i < lines; i++)
            {
                if(matrix[i][base] > 0)
                    break;
            }

            if(i == lines && mode == 1) // Unlimited LP
            {
                unlimitedCertificate(matrix, lines, columns, base, *bases, output);
                return matrix;
            }
        }

        minimum = INT_MAX;
        for(i = 1; i < lines; i++)
        {
            if(matrix[i][base] > 0)
            {
                aux = (matrix[i][columns-1] / matrix[i][base]);
                if(aux < minimum)
                {
                    minimum = aux;
                    pivot = i;
                }
            }
        }

        (*bases)[pivot-1] = base;

        linedivider = matrix[pivot][base];

        for(i = 0; i < columns; i++)
            matrix[pivot][i] /= linedivider;

        for(i = 0; i < lines; i++)
        {
            if(matrix[i][base] != 0 && i != pivot)
            {
                multiplier = -1*(matrix[i][base] / matrix[pivot][base]);

                for(j = 0; j < columns; j++)
                    matrix[i][j] += multiplier * matrix[pivot][j];
            }
        }
    }

    if(mode == 1)
        viableSolution(matrix, lines, columns, *bases, output);

    return matrix;
}

double **dualTableauSolver(double **matrix, int lines, int columns, FILE *output) // Function that solves the given LP in the Dual Tableau algorithm, using Bland's Law
{
    int i, j, base, pivot;
    double minimum, aux, linedivider, multiplier;

    while(1)
    {
        printLineMatrix(matrix, lines, columns, output);

        base = 0;
        for(i = 1; i < lines; i++)
        {
            if(matrix[i][columns-1] < 0)
            {
                base = i;
                break;
            }
        }

        if(base == 0)
            break;

        minimum = INT_MAX;
        for(j = lines-1; j < columns-1; j++)
        {
            if(matrix[base][j] < 0)
            {
                aux = (matrix[0][j] / (-1*(matrix[base][j])));

                if(aux < minimum)
                {
                    minimum = aux;
                    pivot = j;
                }
            }
        }

        linedivider = matrix[base][pivot];

        for(i = 0; i < columns; i++)
            matrix[base][i] /= linedivider;

        printLineMatrix(matrix, lines, columns, output);

        for(i = 0; i < lines; i++)
        {
            if(matrix[i][pivot] != 0 && i != base)
            {
                multiplier = -1*(matrix[i][pivot] / matrix[base][pivot]);

                for(j = 0; j < columns; j++)
                    matrix[i][j] += multiplier * matrix[base][j];
            }
        }
    }
    printLineMatrix(matrix, lines, columns, output);

    return matrix;
}

double **originalIsViable(double **matrix, double *C, int lines, int columns) // Function that overwrite the actual auxiliar C^t for the original C^t in Tableaus
{
    int i;

    for(i = lines-1; i < columns-1; i++)
        matrix[0][i] = C[i-(lines-1)];

    return matrix;
}

void detectNeedOfAuxiliar(double **matrix, int lines, int columns, int mode, FILE *output)
{
    double *Coriginal;
    bool auxiliarflag = 0;
    int i, j;
    int *bases;

    bases = (int*) calloc(lines-1,sizeof(int));
    Coriginal = (double*) calloc((columns-(lines-2)),sizeof(double));

    for(i = lines-2, j = 2; i >= 0; i--, j++)
        bases[i] = columns-j;

    for(i = 0; i < lines; i++)
    {
        if(matrix[i][columns-1] < 0)
        {
            auxiliarflag = 1;
            for(j = 0; j < columns; j++)
                matrix[i][j] *= -1;
        }
    }

    if(auxiliarflag == 1)
    {
        for(j = lines-1; j < columns-(lines-2); j++) // Saves the original C
            Coriginal[j-(lines-1)] = matrix[0][j];

        matrix = buildAuxiliarToTableau(matrix, lines, columns); // Function that builds the auxiliar

        matrix = primalTableauSolver(matrix, lines, columns, 3, &bases, output); // Primal Tableau Simplex algorithm solver

        if(matrix[0][columns-1] < -0.000001)
            unviableCertificate(matrix, lines, output); // Outputs the unviable certificate

        else
        {
            matrix = originalIsViable(matrix, Coriginal, lines, columns); // Objective value is 0, so the original is viable

            matrix = primalTableauSolver(matrix, lines, columns, mode, &bases, output); // Primal Tableau Simplex algorithm solver
        }
    }

    else
        matrix = primalTableauSolver(matrix, lines, columns, mode, &bases, output); // Primal Tableau Simplex algorithm solver
}

int main(int argc, char *argv[])
{
    FILE *input, *output; // Input and output file
    char *matrixinput; // Input matrix
    int lines, columns; // Matrix dimensions
    double **matrix; // Two-dimension array to represent the LP
    int mode, primaldual; // Modes of the execution
    char option; // Primal or dual mode of execution

    matrixinput = (char*) malloc(INT_MAX*sizeof(char));
    input = fopen(argv[1], "r"); // Opens the input file
    output = fopen(argv[2], "w"); // Opens the output file

    fscanf(input, "modo %d", &mode);
    getc(input); // Gets the '\n' token from input

    if(mode == 2)
    {
        fscanf(input, "%c", &option);
        getc(input); // Gets the '\n' token from input
        primaldual = detectPrimalDual(option);
    }

    fscanf(input, "%d", &lines);
    getc(input); // Gets the '\n' token from input
    fscanf(input, "%d", &columns);
    getc(input); // Gets the '\n' token from input
    lines++;
    columns++;

    matrix = matrixAllocation(lines, columns); // Function to allocate the matrix

    fscanf(input, "%s", matrixinput);

    matrixBuilder(matrixinput, matrix); // Function to build the matrix from the input file

    /* First mode implementation */

    if(mode == 1)
    {
        matrix = buildTableau(matrix, &lines, &columns); // Function that builds the Tableau matrix

        detectNeedOfAuxiliar(matrix, lines, columns, mode, output); // Function to detect if the LP input needs an auxiliar
    }

    /* Second mode implementation */

    else if(mode == 2)
    {
        if(primaldual == 0) // Primal solve mode
        {
            matrix = buildTableau(matrix, &lines, &columns); // Function that builds the Tableau matrix

            detectNeedOfAuxiliar(matrix, lines, columns, mode, output); // Function to detect if the LP input needs an auxiliar
        }

        else if(primaldual == 1) // Dual solve mode
        {
            matrix = buildTableau(matrix, &lines, &columns); // Function that builds the Tableau matrix

            matrix = dualTableauSolver(matrix, lines, columns, output); // Dual Tableau Simplex algorithm solver
        }
    }

    fclose(input);
    fclose(output);

    return 0;
}
