#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void copiarVector(int[], int[], int);
int orfila(int[], int[], int[]);
int cuantasPosiciones(int, int[], int);
void rotarVector(int[], int);
double fact(int);
char error_message[30] = "An error has occurred\n";

int main(int argc, char *argv[])
{
    //int *num_ing_dif = NULL;   //vector de cantidad total del ingrediente por ingrediente
    int *pes = (int *)malloc(4 * sizeof(int)); // pp, p2, p3, p4
    int *num_ing_por_plato = NULL;             //numero de ingredientes por plato en una linea en el archivo
    char *ing_dif[] = {};                      //vector de ingredientes diferentes
    //char **ing_dif = NULL;
    int numing = 0;

    FILE *archivo = fopen(argv[1], "r");
    size_t len = 0;
    char *line = NULL;
    if (archivo == NULL)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    int numlinea = 0; // para mirar si es la primera linea o no
    while (getline(&line, &len, archivo) != -1)
    {
        int numtoken = 0; //para conteo de tokens separados por espacios
        char *token = strtok(line, " \n");
        while (token != NULL)
        {
            if (numlinea == 0)
            {
                pes[numtoken] = atoi(token);
                if (numtoken == 0)
                {
                    num_ing_por_plato = (int *)malloc(pes[0] * sizeof(int)); //inicializacion vector
                }
            }
            else if (numlinea == 1)
            {
                if (numtoken == 0)
                {
                    num_ing_por_plato[numlinea - 1] = atoi(token); //redundante
                    numing = num_ing_por_plato[0];                 //inicializacion cantidad de ingredientes
                    //ing_dif = (char **)malloc(numing);             //cuidado //inicializacion vector
                }
                else
                {
                    char *ing = (char *)malloc(20);
                    strcpy(ing, token);
                    ing_dif[numtoken - 1] = ing;
                }
            }
            else
            {
                if (numtoken == 0)
                {
                    num_ing_por_plato[numlinea - 1] = atoi(token); //redundante
                }
                else
                {
                    int itis = 0; //booleano para saber si el ingrediente ya esta en ing_dif
                    for (int i = 0; i < numing; i++)
                    {
                        if (strstr(token, ing_dif[i]) != NULL || strstr(ing_dif[i], token) != NULL)
                        {
                            itis = 1;
                            break;
                        }
                    }
                    if (itis == 0)
                    {
                        numing = numing + 1;
                        //ing_dif = (char **)realloc(ing_dif, numing);
                        char *ing = (char *)malloc(20);
                        strcpy(ing, token);
                        ing_dif[numing - 1] = ing;
                    }
                }
            }
            token = strtok(NULL, " \n");
            numtoken = numtoken + 1;
        }
        numlinea = numlinea + 1;
    }
    fclose(archivo);

    int matriz[numing][pes[0]]; //el vector 2d
    for (int i = 0; i < numing; i++)
    {
        for (int j = 0; j < pes[0]; j++)
        {
            matriz[i][j] = 0;
        }
    }
    archivo = fopen(argv[1], "r");
    len = 0;
    line = NULL;

    numlinea = 0;
    while (getline(&line, &len, archivo) != -1)
    {
        int numtoken = 0; //para conteo de tokens separados por espacios
        char *token = strtok(line, " \n");
        while (token != NULL)
        {
            if (numlinea != 0 && numtoken != 0)
            {
                for (int i = 0; i < numing; i++)
                {
                    if (strstr(token, ing_dif[i]) != NULL || strstr(ing_dif[i], token) != NULL)
                    {
                        matriz[i][numlinea - 1] = 1;
                        break;
                    }
                }
            }
            token = strtok(NULL, " \n");
            numtoken = numtoken + 1;
        }
        numlinea = numlinea + 1;
    }
    fclose(archivo);

    for (int i = 0; i < numing; i++)
    {
        for (int j = 0; j < pes[0]; j++)
        {
            printf("%d", matriz[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < numing; i++)
    {
        printf("numing:%d", numing);
        printf("ing:%s", ing_dif[i]);
        printf("\n");
        free(ing_dif[i]);
    }

    int num = pes[0];
    int *ap = (int *)malloc(num * sizeof(int));
    int *solucion = (int *)malloc(num * sizeof(int));
    int factoriales[num];
    factoriales[0] = 0;
    ap[0] = 0;
    for (int i = 1; i < num; i++)
    {
        factoriales[i] = fact(i);
        ap[i] = i;
        //printf("####%d\n", factoriales[i]);
    }

    int conteo = 0;
    int posiciones = 0;
    int mayor = 0;
    while (1)
    {

        posiciones = cuantasPosiciones(conteo, factoriales, num);

        rotarVector(ap, posiciones);

        /*
        for (int i = 0; i < num; i++)
        {
            printf("%d", ap[i]);
        }
        //printf("\npos=%d", posiciones);
        printf("\n");*/

        int suma = 0;
        for (int i = 0; i < numing; i++)
        {
            suma = suma + orfila(matriz[i], pes, ap);
        }
        if (suma > mayor)
        {
            mayor = suma;
            copiarVector(ap, solucion, num);
        }
        //printf("suma: %d\n", suma);

        conteo = conteo + 1;
        if (conteo == fact(num))
        {
            break;
        }
    }
    printf("\nsolucion: ");
    for (int i = 0; i < num; i++)
    {
        printf("%d", solucion[i]);
    }
    printf("\n");
    printf("mayor: %d\n", mayor);

    //free(ing_dif);
    free(pes);
    free(solucion);
    free(ap);
    free(num_ing_por_plato);
    exit(0);
}

void copiarVector(int fuente[], int destino[], int len)
{
    for (int i = 0; i < len; i++)
    {
        *(destino + i) = *(fuente + i);
    }
}

int orfila(int fila[], int pes[], int ap[])
{
    int pos = 0;
    int suma = 0;
    for (int i = 1; i < 4; i++) //pedido de a
    {
        for (int j = 0; j < pes[i]; j++) //cuantas veces
        {
            int sumaor = 0;
            for (int k = 0; k < i + 1; k++)
            {
                sumaor = sumaor + fila[ap[pos]];
                pos = pos + 1;
            }
            if (sumaor > 0)
            {
                suma = suma + 1;
            }
        }
    }
    //printf("#%d\n",suma);
    return suma;
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