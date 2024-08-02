#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void draw_horizontal_boundary(FILE *fp, int width) {
    fprintf(fp, "+");
    for (int i = 0; i < width; i++) {
        fprintf(fp, "-+");
    }
    fprintf(fp, "\n");
}

void draw_vertical_walls(FILE *fp, int width, int *vertical_walls) {
    fprintf(fp, "|");
    for (int i = 0; i < width - 1; i++) {
        fprintf(fp, " %c", vertical_walls[i] == 1 ? '|' : ' ');
    }
    fprintf(fp, " |\n");
}

void draw_horizontal_walls(FILE *fp, int width, int *horizontal_walls) {
    fprintf(fp, "+");
    for (int i = 0; i < width; i++) {
        fprintf(fp, "%c+", horizontal_walls[i] == 1 ? '-' : ' ');
    }
    fprintf(fp, "\n");
}

void initialize_first_row(int width, int *vertical_walls, int *cell_sets, int *current_set_id, int *last_wall_position) {
    srand(time(NULL));
    for (int i = 0; i < width - 1; i++) {
        vertical_walls[i] = rand() % 2;
        if (vertical_walls[i] == 1) {
            for (int j = *last_wall_position + 1; j <= i; j++) {
                cell_sets[j] = *current_set_id;
            }
            *last_wall_position = i;
            (*current_set_id)++;
        }
    }
    for (int i = *last_wall_position + 1; i < width; i++) {
        cell_sets[i] = *current_set_id;
    }
    (*current_set_id)++;
}

void place_horizontal_walls(int width, int *horizontal_walls, int *cell_sets, int *current_set_id) {
    int previous_set_id = cell_sets[0];
    int vertical_connection_made = 0;

    for (int i = 0; i < width; i++) {
        horizontal_walls[i] = rand() % 2;
        if (horizontal_walls[i] == 0) {
            vertical_connection_made = 1;
        }

        if (i < width - 1 && previous_set_id != cell_sets[i + 1]) {
            if (vertical_connection_made == 0) {
                horizontal_walls[i] = 0;
            } else {
                vertical_connection_made = 0;
            }
            previous_set_id = cell_sets[i + 1];
        }

        if (i == width - 1 && vertical_connection_made == 0) {
            horizontal_walls[i] = 0;
        }

        if (horizontal_walls[i] == 1) {
            cell_sets[i] = *current_set_id;
            (*current_set_id)++;
        }
    }
}

void connect_vertical_sets(int width, int *vertical_walls, int *cell_sets) {
    for (int i = 0; i < width - 1; i++) {
        if (cell_sets[i] != cell_sets[i + 1]) {
            if (rand() % 2 == 1) {
                int previous_set_id = cell_sets[i + 1];
                cell_sets[i + 1] = cell_sets[i];
                for (int j = 0; j < width; j++) {
                    if (cell_sets[j] == previous_set_id) {
                        cell_sets[j] = cell_sets[i];
                    }
                }
                vertical_walls[i] = 0;
            } else {
                vertical_walls[i] = 1;
            }
        } else {
            vertical_walls[i] = 1;
        }
    }
}

void finalize_vertical_connections(int width, int *vertical_walls, int *cell_sets) {
    for (int i = 0; i < width - 1; i++) {
        if (cell_sets[i] != cell_sets[i + 1]) {
            vertical_walls[i] = 0;
            int previous_set_id = cell_sets[i + 1];
            cell_sets[i + 1] = cell_sets[i];
            for (int j = 0; j < width; j++) {
                if (cell_sets[j] == previous_set_id) {
                    cell_sets[j] = cell_sets[i];
                }
            }
        } else {
            vertical_walls[i] = 1;
        }
    }
}

int main() {
    int WIDTH, HEIGHT;
    printf("Enter the width of the maze: ");
    scanf("%d", &WIDTH);
    printf("Enter the height of the maze: ");
    scanf("%d", &HEIGHT);

    int *horizontal_walls = (int *)malloc(WIDTH * sizeof(int));
    int *vertical_walls = (int *)malloc((WIDTH - 1) * sizeof(int));
    int *cell_sets = (int *)malloc(WIDTH * sizeof(int));

    int current_set_id = 0;
    int last_wall_position = -1;

    FILE *fp = fopen("maze.maz", "w");
    if (fp == NULL) {
        perror("Unable to open file!");
        free(horizontal_walls);
        free(vertical_walls);
        free(cell_sets);
        return 1;
    }

    draw_horizontal_boundary(fp, WIDTH);
    initialize_first_row(WIDTH, vertical_walls, cell_sets, &current_set_id, &last_wall_position);
    draw_vertical_walls(fp, WIDTH, vertical_walls);

    for (int k = 1; k < HEIGHT - 1; k++) {
        place_horizontal_walls(WIDTH, horizontal_walls, cell_sets, &current_set_id);
        draw_horizontal_walls(fp, WIDTH, horizontal_walls);
        connect_vertical_sets(WIDTH, vertical_walls, cell_sets);
        draw_vertical_walls(fp, WIDTH, vertical_walls);
    }

    place_horizontal_walls(WIDTH, horizontal_walls, cell_sets, &current_set_id);
    draw_horizontal_walls(fp, WIDTH, horizontal_walls);
    finalize_vertical_connections(WIDTH, vertical_walls, cell_sets);
    draw_vertical_walls(fp, WIDTH, vertical_walls);
    draw_horizontal_boundary(fp, WIDTH);

    fclose(fp);

    free(horizontal_walls);
    free(vertical_walls);
    free(cell_sets);

    return 0;
}
