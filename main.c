#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "planificadores.h"

int leer_csv(const char *nombre_archivo, Proceso procesos[]) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        return 0;
    }

    char linea[100];
    int count = 0;

    fgets(linea, 100, archivo);

    while(fgets(linea, 100, archivo) && count < MAX_PROCESOS) {
        char *token = strtok(linea, ",");
        if (token) {
            procesos[count].id = atoi(token);
        }

        token = strtok(NULL, ",");
        if (token) {
            procesos[count].arrival_time = atoi(token);
        }

        token = strtok(NULL, ",");
        if (token) {
            procesos[count].burst_time = atoi(token);
        }

        count++;
    }

    fclose(archivo);
    return count;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Uso: %s archivo.csv algoritmo [quantum]\n", argv[0]);
        printf("Algoritmos: fcfs | sjf | stcf | rr\n");
        printf("Ejemplo para Round Robin: %s archivo.csv rr 2\n", argv[0]);
        return 1;
    }

    Proceso procesos[MAX_PROCESOS];
    int n_procesos = leer_csv(argv[1], procesos);

    if (n_procesos == 0)
    {
        printf("No se cargaron procesos.\n");
        return 1;
    }

    if (strcmp(argv[2], "fcfs") == 0)
    {
        fcfs(procesos, n_procesos);
    }
    else if (strcmp(argv[2], "sjf") == 0)
    {
        sjf(procesos, n_procesos);
    }
    else if (strcmp(argv[2], "stcf") == 0)
    {
        stcf(procesos, n_procesos);
    }
    else if (strcmp(argv[2], "rr") == 0)
    {
        if (argc < 4)
        {
            printf("Debes especificar el quantum para Round Robin.\n");
            return 1;
        }
        int quantum = atoi(argv[3]);
        round_robin(procesos, n_procesos, quantum);
    }
    else
    {
        printf("Algoritmo no reconocido.\n");
        return 1;
    }

    return 0;
}