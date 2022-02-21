// Elementary Cellular Automaton
// Szymon Golebiowski

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CONFIGURATION
#define CELL_HEIGHT 10
#define CELL_WIDTH 10
#define DEFAULT_ITERATIONS_NUM 50
#define DEFAULT_COLUMNS_NUM 80
#define ITERATION_TIME 0.01

void display_population(int **population, int iteration, int columns_num) {

    ALLEGRO_COLOR cell_color = al_map_rgb(138, 43, 226);

    for (int i = 0; i < iteration; i++) {

        // set vertical coordinates
        int y1 = i * CELL_HEIGHT;
        int y2 = y1 + CELL_HEIGHT;

        for (int j = 0; j < columns_num; j++) {

            // set horizontal coordinates
            int x1 = j * CELL_WIDTH;
            int x2 = x1 + CELL_WIDTH;

            if (population[i][j] == 1) {
                al_draw_filled_rectangle(x1, y1, x2, y2, cell_color);
            }
        }
    }
}

void visualize_simulation(int **population, int iterations_num, int rule,
                          int population_size, int columns_num) {

    al_init();                  // initialize Allegro library
    al_install_keyboard();      // initialize keyboard handling
    al_init_primitives_addon(); // initialize shapes drawing


    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);

    ALLEGRO_EVENT_QUEUE *events_queue = al_create_event_queue();

    // create a new window
    const int window_width = CELL_WIDTH * columns_num + 200;
    const int window_height = CELL_HEIGHT * iterations_num + 50;

    ALLEGRO_DISPLAY *disp = al_create_display(window_width, window_height);

    // read a default font
    ALLEGRO_FONT *font = al_create_builtin_font();

    al_register_event_source(events_queue, al_get_keyboard_event_source());

    al_register_event_source(events_queue, al_get_display_event_source(disp));

    al_register_event_source(events_queue, al_get_timer_event_source(timer));

    bool refresh = 1;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    double time, previous_time = al_get_time();
    int iteration = 0;
    char text[100];

    while (1) {

        al_wait_for_event(events_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            refresh = 1;
        } else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) ||
                   (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) {
            break;
        }

        if (refresh && al_is_event_queue_empty(events_queue)) {

            al_clear_to_color(al_map_rgb(0, 0, 0));

            time = al_get_time();
            if (iteration < iterations_num && time > ITERATION_TIME + previous_time) {
                previous_time = time;
                iteration++;
            }

            sprintf(text, "ITERATION NUM: %d / %d", iteration, iterations_num);
            int x1 = CELL_WIDTH * columns_num + 15, y1 = 20;
            al_draw_text(font, al_map_rgb(255, 255, 255), x1, y1, 0, text);

            y1 += 20;
            sprintf(text, "RULE: %d", rule);
            al_draw_text(font, al_map_rgb(255, 255, 255), x1, y1, 0, text);

            y1 += 20;
            sprintf(text, "POPULATION SIZE: %d", population_size);
            al_draw_text(font, al_map_rgb(255, 255, 255), x1, y1, 0, text);

            y1 += 20;
            sprintf(text, "COLUMNS NUMBER: %d", columns_num);
            al_draw_text(font, al_map_rgb(255, 255, 255), x1, y1, 0, text);

            if (iteration == iterations_num) {

                x1 = 40;
                y1 = CELL_HEIGHT * iterations_num + 20;
                sprintf(text,
                        "SIMULATION FINISHED. PRESS ANY KEY TO CLOSE THE WINDOW");
                al_draw_text(font, al_map_rgb(255, 255, 255), x1, y1, 0, text);
            }

            display_population(population, iteration, columns_num);

            al_flip_display();
            refresh = 0;
        }
    }

    // close the window and finalize the Allegro library
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(events_queue);
}

int **create_population(int iterations_num, int population_size, int columns_num) {

    int **population = (int **)malloc(iterations_num * sizeof(int *));
    for (int i = 0; i < iterations_num; i++) {

        population[i] = (int *)malloc(columns_num * sizeof(int));

        for (int j = 0; j < columns_num; j++) {
            population[i][j] = 0;
        }
    }

    int cell;
    for (int i = 0; i < population_size; i++) {

        do {
            cell = rand() % columns_num;
        } while (population[0][cell] == 1);

        population[0][cell] = 1;
    }

    return population;
}


void delete_population(int **population, int iterations_num) {

    for (int i = 0; i < iterations_num; i++) {
        free(population[i]);
    }

    free(population);
}


int calculate_cell(int upper_left, int upper_middle, int upper_right, int rule) {

    int upper_cells_type = 4 * upper_left + 2 * upper_middle + upper_right;

    if (rule & (1 << upper_cells_type)) {
        return 1;
    }

    return 0;
}

void calculate_iteration(int **population, int iteration, int rule, int columns_num) {

    int upper_left, upper_middle, upper_right;
    int upper_left_num, upper_middle_num, upper_right_num;
    int current_cell;

    for (int cell_num = 0; cell_num < columns_num; cell_num++) {

        upper_left_num = (cell_num - 1 + columns_num) % columns_num;
        upper_middle_num = cell_num;
        upper_right_num = (cell_num + 1) % columns_num;

        upper_left = population[iteration - 1][upper_left_num];
        upper_middle = population[iteration - 1][upper_middle_num];
        upper_right = population[iteration - 1][upper_right_num];

        current_cell = calculate_cell(upper_left, upper_middle, upper_right, rule);

        population[iteration][cell_num] = current_cell;
    }
}

void run_simulation(int rule, int population_size, int iterations_num,
                    int columns_num) {

    int **population =
        create_population(iterations_num, population_size, columns_num);

    for (int iteration = 1; iteration < iterations_num; iteration++) {

        calculate_iteration(population, iteration, rule, columns_num);
    }

    visualize_simulation(population, iterations_num, rule, population_size,
                         columns_num);

    delete_population(population, iterations_num);
}

int random_number(int min, int max) { return (rand() % (max - min + 1)) + min; }

int main() {

    srand(time(NULL));

    int population_size = 10;
    int rule = 122;

    int columns_num = DEFAULT_COLUMNS_NUM;
    int iterations_num = DEFAULT_ITERATIONS_NUM;

    // run_simulation(rule, population_size, iterations_num, columns_num);

    printf("population size: %d\n", population_size);
    printf("rule: %d\n", rule);
    printf("iterations_num: %d\n", iterations_num);
    printf("columns_num: %d\n", columns_num);

    return 0;
}