#ifndef PLANIFICADORES_H
#define PLANIFICADORES_H

#define MAX_PROCESOS 12

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
} Proceso;

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int start_time;
    int finish_time;
    int response_time;
    int turnaround_time;
    int waiting_time;
} Resultado;

void fcfs(Proceso procesos[], int n);
void sjf(Proceso procesos[], int n);
void stcf(Proceso procesos[], int n);
void round_robin(Proceso procesos[], int n, int quantum);

#endif