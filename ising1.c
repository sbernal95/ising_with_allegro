#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define L 128  // Tamaño de la cuadrícula LxL
#define STEPS 100000  // Número de pasos de Monte Carlo
#define SCALE 4 //Tamaño de spin por pantalla

// Función para inicializar la cuadrícula con espines aleatorios
void initialize_spins(int *spins[L]) {
    for (int i = 0; i < L; i++) {
        spins[i] = malloc(L * sizeof(int));
        
        for (int j = 0; j < L; j++) {
            spins[i][j] = (rand() % 2) * 2 - 1;  // -1 o 1 aleatoriamente
        }

    }
}

// Función para calcular la energía del sistema
int calculate_energy(int *spins[L], int x, int y) {
    int up = spins[(x - 1 + L) % L][y];
    int down = spins[(x + 1) % L][y];
    int left = spins[x][(y - 1 + L) % L];
    int right = spins[x][(y + 1) % L];
    return spins[x][y] * (up + down + left + right);
}



// Función principal para ejecutar el algoritmo Metropolis-Hastings
void metropolis_hastings(int *spins[L], float t) {
    int x, y;
    
    for (int step = 0; step < STEPS; step++) {
        x = rand() % L;
        y = rand() % L;

        int deltaE = 2 * calculate_energy(spins, x, y);
        if (deltaE < 0 || (rand() / (double)RAND_MAX) < exp(-deltaE / t)) {
            spins[x][y] *= -1;  // Invertir el espín
        }

    }
}

float calculate_magnetization(int *spins[L]) {
    int sum = 0;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            sum += spins[i][j];
        }
    }
    return fabs((float) sum / (L * L));
}

// Función para imprimir la configuración de espines
void print_spins(int *spins[L]) {
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            /*printf("%2d ", spins[i][j]);*/
            if (spins[i][j] == 1) al_draw_filled_rectangle(i * SCALE, j * SCALE, (i + 1) * SCALE, (j + 1) * SCALE, al_map_rgb(255, 0, 0));
            else al_draw_filled_rectangle(i * SCALE, j * SCALE, (i + 1) * SCALE, (j + 1) * SCALE, al_map_rgb(0, 0, 0));
        }
    }
}

int main(int argc, char* argv[]) {

    float temp;

    if(argc > 1){
        temp = atof(argv[1]);
    } else {
        temp = 1.0;
    }

    printf("Programa inicializado.\n");

    ////////////////////ALLEGRO ZONE////////////////////
    al_init();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT ev;

    if (!al_init()) {
        fprintf(stderr, "Error al inicializar Allegro.\n");
        return -1;
    }

    display = al_create_display(SCALE * L, SCALE * L);
    if (!display) {
        fprintf(stderr, "Error al crear la ventana de visualización.\n");
        return -1;
    }
    
    al_init_primitives_addon();

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    
    if (!event_queue) {
        fprintf(stderr, "Error al crear la cola de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    printf("Cola de eventos creada.\n");

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    printf("Eventos registrados.\n");

////////////////////ALLEGRO ZONE////////////////////
    printf("Allegro inicializado.\n");

    float ms_photo_delay = 0.001;
    float mag;

    //double t1, t2, t_total, dt;

    srand(time(NULL));
    int *spins[L];

    initialize_spins(spins);
    print_spins(spins);

    int i;
    for (i = 0; i < 5000; i++) {

        if(al_get_next_event(event_queue, &ev)){
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }}
        
        //t1 = al_get_time();

        metropolis_hastings(spins, temp);
        print_spins(spins);
        mag = calculate_magnetization(spins);

        //t2 = al_get_time();

        //t_total += (t2 - t1);

        //dt = (t_total / (i + 1));

        //printf("mag: %f, calc_time: %f, Temp: %f\n", mag, dt, temp);
        printf("mag: %f, Temp: %f\n", mag, temp);

        al_flip_display(); // Actualiza la pantalla
        al_rest(ms_photo_delay); // Espera un poco para controlar la velocidad de actualización

    }

    printf("Pasos Monte Carlo total: %d", STEPS * i);

    return 0;
}
