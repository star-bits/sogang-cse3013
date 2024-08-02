#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Structure to represent an edge in the graph
typedef struct {
    int weight;
    int x1, y1;
    int x2, y2;
    int wall_x, wall_y;
} Edge;

// Function prototypes
void create_grid(char** grid, int width, int height);
void save_to_file(char** grid, int width, int height, const char* filename);
void add_edges(int x, int y, int width, int height, bool** visited, Edge** edges, int* edge_count);
void generate_maze(char** grid, int width, int height);

int main() {
    int width, height;
    printf("Enter the width of the maze: ");
    scanf("%d", &width);
    printf("Enter the height of the maze: ");
    scanf("%d", &height);

    // Dynamically allocate memory for the grid
    char** grid = malloc((2 * height + 1) * sizeof(char*));
    for (int i = 0; i < 2 * height + 1; i++) {
        grid[i] = malloc((2 * width + 1) * sizeof(char));
    }

    create_grid(grid, width, height);
    generate_maze(grid, width, height);
    save_to_file(grid, width, height, "maze.maz");

    // Free allocated memory
    for (int i = 0; i < 2 * height + 1; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}

// Function to create the initial grid
void create_grid(char** grid, int width, int height) {
    for (int i = 0; i < 2 * height + 1; i++) {
        for (int j = 0; j < 2 * width + 1; j++) {
            if (i % 2 == 0 && j % 2 == 1) {
                grid[i][j] = '-';
            } else if (i % 2 == 1 && j % 2 == 0) {
                grid[i][j] = '|';
            } else if (i % 2 == 1 && j % 2 == 1) {
                grid[i][j] = ' ';
            } else {
                grid[i][j] = '+';
            }
        }
    }
}

// Function to save the grid to a file
void save_to_file(char** grid, int width, int height, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < 2 * height + 1; i++) {
            for (int j = 0; j < 2 * width + 1; j++) {
                fputc(grid[i][j], file);
            }
            fputc('\n', file);
        }
        fclose(file);
    } else {
        printf("Error opening file for writing.\n");
    }
}

// Function to add edges to the edge list
void add_edges(int x, int y, int width, int height, bool** visited, Edge** edges, int* edge_count) {
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        if (nx >= 0 && nx < width && ny >= 0 && ny < height && !visited[ny][nx]) {
            int wall_x = 2 * y + 1 + directions[i][1];
            int wall_y = 2 * x + 1 + directions[i][0];
            int weight = rand() % 100 + 1;
            edges[*edge_count] = malloc(sizeof(Edge));
            *edges[*edge_count] = (Edge){weight, x, y, nx, ny, wall_x, wall_y};
            (*edge_count)++;
        }
    }
}

// Function to generate the maze using Prim's algorithm
void generate_maze(char** grid, int width, int height) {
    bool** visited = malloc(height * sizeof(bool*));
    for (int i = 0; i < height; i++) {
        visited[i] = malloc(width * sizeof(bool));
        for (int j = 0; j < width; j++) {
            visited[i][j] = false;
        }
    }

    Edge** edges = malloc(width * height * 4 * sizeof(Edge*));
    int edge_count = 0;

    srand(time(NULL));

    int start_x = 0, start_y = 0;
    visited[start_y][start_x] = true;
    add_edges(start_x, start_y, width, height, visited, edges, &edge_count);

    while (edge_count > 0) {
        int min_index = 0;
        for (int i = 1; i < edge_count; i++) {
            if (edges[i]->weight < edges[min_index]->weight) {
                min_index = i;
            }
        }

        Edge* min_edge = edges[min_index];
        edges[min_index] = edges[--edge_count];

        if (!visited[min_edge->y2][min_edge->x2]) {
            grid[min_edge->wall_x][min_edge->wall_y] = ' ';
            visited[min_edge->y2][min_edge->x2] = true;
            add_edges(min_edge->x2, min_edge->y2, width, height, visited, edges, &edge_count);
        }

        free(min_edge); // Free the used edge
    }

    // Free allocated memory
    for (int i = 0; i < height; i++) {
        free(visited[i]);
    }
    free(visited);

    free(edges); // Free edge pointers array
}
