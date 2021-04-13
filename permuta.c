#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cuantasPosiciones(int, int[], int);
void rotarVector(int[], int);
double fact(int);

int main(int argc, char *argv[])
{
    int num = atoi(argv[1]);
    int vector[num];
    //int contadores[num];
    int factoriales[num];
    factoriales[0] = 0;
    vector[0] = 0;
    for (int i = 1; i < num; i++)
    {
        vector[i] = i;
        //contadores[i] = 0;
        factoriales[i] = fact(i);
        printf("####%d\n", factoriales[i]);
    }

    int conteo = 0;
    int posiciones = 0;
    while (1)
    {

        posiciones = cuantasPosiciones(conteo, factoriales, num);

        rotarVector(vector, posiciones);

        for (int i = 0; i < num; i++)
        {
            printf("%d", vector[i]);
        }
        //printf("\npos=%d", posiciones);
        printf("\n");

        conteo = conteo + 1;
        if (conteo == fact(num))
        {
            break;
        }
    }
}

int cuantasPosiciones(int conteo, int vectorFactoriales[], int longitudVector)
{
    if (conteo == 0)
    {
        return 0;
    }
    for (int i = longitudVector - 1; i > 0; i--)
    {
        if (conteo % vectorFactoriales[i] == 0)
        {
            return i + 1;
        }
    }
    return 0;
}

void rotarVector(int vector[], int posiciones)
{
    int p = posiciones / 2;
    int aux = 0;

    for (int i = 0; i < p; i++)
    {
        aux = vector[i];
        vector[i] = vector[posiciones - 1 - i];
        vector[posiciones - 1 - i] = aux;
    }
}

double fact(int num)
{
    if (num == 1 || num == 0)
    {
        return 1;
    }
    return num * fact(num - 1);
}