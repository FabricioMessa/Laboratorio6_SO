#include "planificadores.h"
#include <stdio.h>
#include <string.h>

/*============================================================================================*/
/*====================================== FIFO ================================================*/
/*============================================================================================*/
void fcfs(Proceso procesos[], int n) {
    Resultado resultados[MAX_PROCESOS];
    int tiempo_actual = 0;
    int gantt[1000]; 
    int tiempo_total = 0;

    memset(gantt, -1, sizeof(gantt));

    for (int i = 0; i < n; i++) {
        if (procesos[i].arrival_time > tiempo_actual) {
            for (int t = tiempo_actual; t < procesos[i].arrival_time; t++) {
                gantt[t] = -1;
            }
            tiempo_actual = procesos[i].arrival_time;
        }

        resultados[i].id = procesos[i].id;
        resultados[i].arrival_time = procesos[i].arrival_time;
        resultados[i].burst_time = procesos[i].burst_time;
        resultados[i].start_time = tiempo_actual;
        resultados[i].finish_time = tiempo_actual + procesos[i].burst_time;
        resultados[i].response_time = resultados[i].start_time - procesos[i].arrival_time;
        resultados[i].turnaround_time = resultados[i].finish_time - procesos[i].arrival_time;
        resultados[i].waiting_time = resultados[i].turnaround_time - procesos[i].burst_time;

        for (int t = tiempo_actual; t < tiempo_actual + procesos[i].burst_time; t++) {
            gantt[t] = procesos[i].id;
        }

        tiempo_actual += procesos[i].burst_time;
    }
    tiempo_total = tiempo_actual;

    printf("\nFCFS - Tabla de resultados:\n");
    printf("ID | Llegada | Rafaga | Inicio | Fin | Respuesta | Retorno | Espera\n");
    printf("---------------------------------------------------------------\n");
    double sum_response = 0, sum_turnaround = 0, sum_waiting = 0;
    
    for (int i = 0; i < n; i++) {
        printf("%2d | %7d | %6d | %6d | %3d | %9d | %7d | %6d\n",
               resultados[i].id,
               resultados[i].arrival_time,
               resultados[i].burst_time,
               resultados[i].start_time,
               resultados[i].finish_time,
               resultados[i].response_time,
               resultados[i].turnaround_time,
               resultados[i].waiting_time);
        sum_response += resultados[i].response_time;
        sum_turnaround += resultados[i].turnaround_time;
        sum_waiting += resultados[i].waiting_time;
    }

    printf("---------------------------------------------------------------\n");
    printf("P |          |        |        |     | %9.2f | %7.2f | %6.2f\n\n", sum_response / n, sum_turnaround / n, sum_waiting / n);

    printf("FCFS - Diagrama de Gantt:\n");
    for (int t = 0; t < tiempo_total; t++) {
        printf("[%d]", t);
        if (gantt[t] == -1) {
            printf("--IDLE--");
        } else {
            printf("--P%d--", gantt[t]);
        }
    }
    printf("[%d]\n", tiempo_total);

    printf("Average Waiting Time: %.2f\n", sum_waiting / n);
    printf("Average Turnaround Time: %.2f\n", sum_turnaround / n);
}

/*============================================================================================*/
/*======================================= SJF ================================================*/
/*============================================================================================*/
void sjf(Proceso procesos[], int n) {
    Resultado resultados[MAX_PROCESOS];
    int ejecutado[MAX_PROCESOS] = {0};
    int procesos_restantes = n;
    int tiempo_actual = 0;
    int gantt[1000];
    memset(gantt, -1, sizeof(gantt));

    int tiempo_total = 0;

    while (procesos_restantes > 0) {
        int idx = -1;
        int min_burst = 1e9;
        for (int i = 0; i < n; i++) {
            if (!ejecutado[i] && procesos[i].arrival_time <= tiempo_actual) {
                if (procesos[i].burst_time < min_burst || (procesos[i].burst_time == min_burst && procesos[i].id < procesos[idx].id)) {
                    min_burst = procesos[i].burst_time;
                    idx = i;
                }
            }
        }
    
        if (idx == -1) {
            gantt[tiempo_actual] = -1;
            tiempo_actual++;
            continue;
        }

        resultados[idx].id = procesos[idx].id;
        resultados[idx].arrival_time = procesos[idx].arrival_time;
        resultados[idx].burst_time = procesos[idx].burst_time;
        resultados[idx].start_time = tiempo_actual;
        resultados[idx].finish_time = tiempo_actual + procesos[idx].burst_time;
        resultados[idx].response_time = resultados[idx].start_time - procesos[idx].arrival_time;
        resultados[idx].turnaround_time = resultados[idx].finish_time - procesos[idx].arrival_time;
        resultados[idx].waiting_time = resultados[idx].turnaround_time - procesos[idx].burst_time;

        for (int t = tiempo_actual; t < tiempo_actual + procesos[idx].burst_time; t++) {
            gantt[t] = procesos[idx].id;
        }

        tiempo_actual += procesos[idx].burst_time;
        ejecutado[idx] = 1;
        procesos_restantes--;
    }

    tiempo_total = tiempo_actual;

    printf("\nSJF - Tabla de resultados:\n");
    printf("ID | Llegada | Rafaga | Inicio | Fin | Respuesta | Retorno | Espera\n");
    printf("---------------------------------------------------------------\n");
    double sum_response = 0, sum_turnaround = 0, sum_waiting = 0;
    for (int i = 0; i < n; i++) {
        printf("%2d | %7d | %6d | %6d | %3d | %9d | %7d | %6d\n",
               resultados[i].id,
               resultados[i].arrival_time,
               resultados[i].burst_time,
               resultados[i].start_time,
               resultados[i].finish_time,
               resultados[i].response_time,
               resultados[i].turnaround_time,
               resultados[i].waiting_time);
        sum_response += resultados[i].response_time;
        sum_turnaround += resultados[i].turnaround_time;
        sum_waiting += resultados[i].waiting_time;
    }
    printf("---------------------------------------------------------------\n");
    printf("P |          |        |        |     | %9.2f | %7.2f | %6.2f\n\n", sum_response / n, sum_turnaround / n, sum_waiting / n);

    printf("SJF - Diagrama de Gantt:\n");
    for (int t = 0; t < tiempo_total; t++) {
        printf("[%d]", t);
        if (gantt[t] == -1) {
            printf("--IDLE--");
        } else {
            printf("--P%d--", gantt[t]);
        }
    }
    printf("[%d]\n", tiempo_total);

    printf("Average Waiting Time: %.2f\n", sum_waiting / n);
    printf("Average Turnaround Time: %.2f\n", sum_turnaround / n);
}

/*============================================================================================*/
/*====================================== STCF ================================================*/
/*============================================================================================*/
void stcf(Proceso procesos[], int n) {
    int tiempos_restantes[MAX_PROCESOS];
    int completados[MAX_PROCESOS] = {0};
    Resultado resultados[MAX_PROCESOS];
    int procesos_restantes = n;
    int tiempo_actual = 0;
    int gantt[1000];
    memset(gantt, -1, sizeof(gantt));

    for (int i = 0; i < n; i++) {
        tiempos_restantes[i] = procesos[i].burst_time;
        resultados[i].id = procesos[i].id;
        resultados[i].arrival_time = procesos[i].arrival_time;
        resultados[i].burst_time = procesos[i].burst_time;
        resultados[i].start_time = -1;
    }

    while (procesos_restantes > 0) {
        int idx = -1;
        int min_restante = 1e9;
        for (int i = 0; i < n; i++) {
            if (!completados[i] && procesos[i].arrival_time <= tiempo_actual && tiempos_restantes[i] > 0) {
                if (tiempos_restantes[i] < min_restante || (tiempos_restantes[i] == min_restante && procesos[i].id < procesos[idx].id)) {
                    min_restante = tiempos_restantes[i];
                    idx = i;
                }
            }
        }
        
        if (idx == -1) {
            gantt[tiempo_actual] = -1;
            tiempo_actual++;
            continue;
        }

        if (resultados[idx].start_time == -1) {
            resultados[idx].start_time = tiempo_actual;
            resultados[idx].response_time = tiempo_actual - procesos[idx].arrival_time;
        }

        gantt[tiempo_actual] = procesos[idx].id;
        tiempos_restantes[idx]--;
        tiempo_actual++;

        if (tiempos_restantes[idx] == 0) {
            completados[idx] = 1;
            procesos_restantes--;
            resultados[idx].finish_time = tiempo_actual;
            resultados[idx].turnaround_time = resultados[idx].finish_time - procesos[idx].arrival_time;
            resultados[idx].waiting_time = resultados[idx].turnaround_time - resultados[idx].burst_time;
        }
    }

    int tiempo_total = tiempo_actual;

    printf("\nSTCF - Tabla de resultados:\n");
    printf("ID | Llegada | Rafaga | Inicio | Fin | Respuesta | Retorno | Espera\n");
    printf("---------------------------------------------------------------\n");
    double sum_response = 0, sum_turnaround = 0, sum_waiting = 0;
    for (int i = 0; i < n; i++) {
        printf("%2d | %7d | %6d | %6d | %3d | %9d | %7d | %6d\n",
               resultados[i].id,
               resultados[i].arrival_time,
               resultados[i].burst_time,
               resultados[i].start_time,
               resultados[i].finish_time,
               resultados[i].response_time,
               resultados[i].turnaround_time,
               resultados[i].waiting_time);
        sum_response += resultados[i].response_time;
        sum_turnaround += resultados[i].turnaround_time;
        sum_waiting += resultados[i].waiting_time;
    }

    printf("---------------------------------------------------------------\n");
    printf("P |          |        |        |     | %9.2f | %7.2f | %6.2f\n\n", sum_response / n, sum_turnaround / n, sum_waiting / n);

    printf("STCF - Diagrama de Gantt:\n");
    for (int t = 0; t < tiempo_total; t++) {
        printf("[%d]", t);
        if (gantt[t] == -1) {
            printf("--IDLE--");
        } else {
            printf("--P%d--", gantt[t]);
        }
    }
    printf("[%d]\n", tiempo_total);

    printf("Average Waiting Time: %.2f\n", sum_waiting / n);
    printf("Average Turnaround Time: %.2f\n", sum_turnaround / n);
}

/*============================================================================================*/
/*======================================= RR =================================================*/
/*============================================================================================*/
void round_robin(Proceso procesos[], int n, int quantum) {
    Resultado resultados[MAX_PROCESOS];
    int tiempos_restantes[MAX_PROCESOS];
    int completados[MAX_PROCESOS] = {0};
    int tiempo_actual = 0;
    int procesos_restantes = n;
    int en_cola[MAX_PROCESOS] = {0};
    int cola[1000];
    int frente = 0, fin = 0;
    int gantt[1000];
    memset(gantt, -1, sizeof(gantt));

    for (int i = 0; i < n; i++) {
        tiempos_restantes[i] = procesos[i].burst_time;
        resultados[i].id = procesos[i].id;
        resultados[i].arrival_time = procesos[i].arrival_time;
        resultados[i].burst_time = procesos[i].burst_time;
        resultados[i].start_time = -1;
    }

    for (int i = 0; i < n; i++) {
        if (procesos[i].arrival_time == 0) {
            cola[fin++] = i;
            en_cola[i] = 1;
        }
    }

    while (procesos_restantes > 0) {
        if (frente == fin) {
            gantt[tiempo_actual] = -1;
            tiempo_actual++;
            for (int i = 0; i < n; i++) {
                if (!en_cola[i] && procesos[i].arrival_time <= tiempo_actual && tiempos_restantes[i] > 0) {
                    cola[fin++] = i;
                    en_cola[i] = 1;
                }
            }
            continue;
        }

        int idx = cola[frente++];

        if (resultados[idx].start_time == -1) {
            resultados[idx].start_time = tiempo_actual;
            resultados[idx].response_time = tiempo_actual - procesos[idx].arrival_time;
        }

        int tiempo_ejecucion = (tiempos_restantes[idx] < quantum) ? tiempos_restantes[idx] : quantum;
        for (int i = 0; i < tiempo_ejecucion; i++) {
            gantt[tiempo_actual + i] = procesos[idx].id;
        }

        tiempos_restantes[idx] -= tiempo_ejecucion;
        int tiempo_previo = tiempo_actual;
        tiempo_actual += tiempo_ejecucion;

        for (int t = 1; t <= tiempo_ejecucion; t++) {
            for (int i = 0; i < n; i++) {
                if (!en_cola[i] && procesos[i].arrival_time == tiempo_previo + t && tiempos_restantes[i] > 0) {
                    cola[fin++] = i;
                    en_cola[i] = 1;
                }
            }
        }

        if (tiempos_restantes[idx] == 0) {
            completados[idx] = 1;
            procesos_restantes--;
            resultados[idx].finish_time = tiempo_actual;
            resultados[idx].turnaround_time = resultados[idx].finish_time - procesos[idx].arrival_time;
            resultados[idx].waiting_time = resultados[idx].turnaround_time - resultados[idx].burst_time;
        } else {
            cola[fin++] = idx;
        }
    }

    int tiempo_total = tiempo_actual;

    printf("\nRound Robin (quantum=%d) - Tabla de resultados:\n", quantum);
    printf("ID | Llegada | Rafaga | Inicio | Fin | Respuesta | Retorno | Espera\n");
    printf("---------------------------------------------------------------\n");
    double sum_response = 0, sum_turnaround = 0, sum_waiting = 0;
    for (int i = 0; i < n; i++) {
        printf("%2d | %7d | %6d | %6d | %3d | %9d | %7d | %6d\n",
               resultados[i].id,
               resultados[i].arrival_time,
               resultados[i].burst_time,
               resultados[i].start_time,
               resultados[i].finish_time,
               resultados[i].response_time,
               resultados[i].turnaround_time,
               resultados[i].waiting_time);
        sum_response += resultados[i].response_time;
        sum_turnaround += resultados[i].turnaround_time;
        sum_waiting += resultados[i].waiting_time;
    }

    printf("---------------------------------------------------------------\n");
    printf("P |          |        |        |     | %9.2f | %7.2f | %6.2f\n\n", sum_response / n, sum_turnaround / n, sum_waiting / n);

    printf("Round Robin - Diagrama de Gantt:\n");
    for (int t = 0; t < tiempo_total; t++) {
        printf("[%d]", t);
        if (gantt[t] == -1) {
            printf("--IDLE--");
        } else {
            printf("--P%d--", gantt[t]);
        }
    }
    
    printf("[%d]\n", tiempo_total);

    printf("Average Waiting Time: %.2f\n", sum_waiting / n);
    printf("Average Turnaround Time: %.2f\n", sum_turnaround / n);
}