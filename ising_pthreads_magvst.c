#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define L 128  // Tamaño de la cuadrícula LxL
#define STEPS 100000000  // Número de pasos de Monte Carlo
#define N_THR 4  // Número de hilos

//Esta es la estructura de los datos correspondientes a cada hilo
typedef struct {
    int **spins;
    float t;
    int start;
    int end;
} ThreadData;

//Esta función inicializa la matriz de spines de forma random
void initialize_spins(int **spins) {
    srand(123456);
    for (int i = 0; i < L; i++) {
        spins[i] = malloc(L * sizeof(int));
        for (int j = 0; j < L; j++) {
            spins[i][j] = (rand() % 2) * 2 - 1;  // -1 o 1 aleatoriamente
        }
    }
}

//Esta función calcula la magnetización total de la matriz
float calculate_magnetization(int **spins) {
    int sum = 0;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            sum += spins[i][j];
        }
    }
    return fabs((float) sum / (L * L));
}

//Est función calcula la energía circundante al spin por actualizar
int calculate_energy(int **spins, int x, int y) {
    int up = spins[(x - 1 + L) % L][y];
    int down = spins[(x + 1) % L][y];
    int left = spins[x][(y - 1 + L) % L];
    int right = spins[x][(y + 1) % L];
    return spins[x][y] * (up + down + left + right);
}

//Esta función ejecurta el algoritmo en forma de hilo
void* metropolis_hastings_thread(void *arg) {
    //Se reciben los datos para el hilo
    ThreadData *data = (ThreadData*) arg;
    int **spins = data->spins;
    float t = data->t;
    int start = data->start;
    int end = data->end;

    //Se asigna la seed especifica del hilo
    unsigned int seed = pthread_self();

    //Se ejecuta el ciclo critico del algoritmo dentro de los limites asinados al hilo
    for (int step = start; step < end; step++) {
        int x = rand_r(&seed) % L;
        int y = rand_r(&seed) % L;
        double p = rand_r(&seed) / (double)RAND_MAX;

        int deltaE = 2 * calculate_energy(spins, x, y);
        if (deltaE < 0 || p < exp(-deltaE / t)) {
            spins[x][y] *= -1;
        }
    }


    pthread_exit(NULL);
}

//Esta función reparte el algoritmo entre el numero de hilos definido
void metropolis_hastings_hilachudo(int **spins, float t) {
    pthread_t threads[N_THR]; //Arreglo de los hilos
    ThreadData thread_data[N_THR]; //Arreglo de los datos por cada hilo
    int steps_per_thread = STEPS / N_THR; //Esta es la candidad de pasos por cada hilo

    //En este ciclo se asignan para cada hilo los datos y la tarea a realizar
    for (int i = 0; i < N_THR; i++) {
        thread_data[i].spins = spins;
        thread_data[i].t = t;
        thread_data[i].start = i * steps_per_thread;
        thread_data[i].end = (i + 1) * steps_per_thread;

        pthread_create(&threads[i], NULL, metropolis_hastings_thread, &thread_data[i]);
    }

    //Se espera que terminen todos los hilos en ejecución
    for (int i = 0; i < N_THR; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(int argc, char* argv[]) {
    //Se definen la variable para la temperatura
    float temp, mag;

    //Se define la variable del archivo de salida
    //Cada columna corresponderá a cada valor que toma la temperatura
    FILE *fp;
    fp = fopen("magvst.csv", "w");

    //Se define la matriz de spines
    int *spins[L];

    printf("#->MonteCarlo Steps: %.2e\n", (float) STEPS);
    printf("#->Number of Threads: %d\n", (int) N_THR);
    printf("Running...\n");

    for(temp = 0.1; temp < 4; temp += 0.1) fprintf(fp, "%f,", temp);
    fprintf(fp, "\b\n");

    //Se repetirá el proceso 10 veces para obtener 10 filas en el archivo
    for(int i = 0; i < 10; i++){
        printf("Obteniendo fila %d/10\n", i);

        for(temp = 0.1; temp < 4; temp += 0.1){
            //Para cada temperatura se inicializa una nueva matriz
            initialize_spins(spins);

            //Inicio del algoritmo
            metropolis_hastings_hilachudo(spins, temp);

            //Se calcula la magnetización de la matriz
            mag = calculate_magnetization(spins);

            //Se regristra la magnetización en el archivo
            fprintf(fp, "%f,", mag);
            printf("mag_f: %f, temp: %f\n", mag, temp);
        }

        //Se finaliza la fila con un salto de linea
        fprintf(fp, "\b\n");
    }

    //Se libera la memoria utilizada
    for (int i = 0; i < L; i++) {
        free(spins[i]);
    }

    fclose(fp);

    return 0;
}
