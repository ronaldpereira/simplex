#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE *entrada, *saida;
    entrada = fopen(argv[1], "r");
    saida = fopen(argv[2], "w");

    fclose(entrada);
    fclose(saida);

    return 0;
}
