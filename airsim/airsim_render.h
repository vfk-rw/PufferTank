#include <stdio.h>
#include <string.h>
#include "airsim.h"

#define GRID_WIDTH 80
#define GRID_HEIGHT 24

typedef struct TextClient {
    char grid[GRID_HEIGHT][GRID_WIDTH];
    float scale_x;
    float scale_y;
} TextClient;

TextClient* make_client(AirDefense* env) {
    TextClient* client = (TextClient*)calloc(1, sizeof(TextClient));
    client->scale_x = (float)GRID_WIDTH / env->config.width;
    client->scale_y = (float)GRID_HEIGHT / env->config.height;
    return client;
}

void close_client(TextClient* client) {
    free(client);
}

void clear_grid(TextClient* client) {
    for(int y = 0; y < GRID_HEIGHT; y++) {
        for(int x = 0; x < GRID_WIDTH; x++) {
            client->grid[y][x] = ' ';
        }
    }
}

void set_char(TextClient* client, float x, float y, char c) {
    int grid_x = (int)(x * client->scale_x);
    int grid_y = (int)(y * client->scale_y);
    
    if(grid_x >= 0 && grid_x < GRID_WIDTH && 
       grid_y >= 0 && grid_y < GRID_HEIGHT) {
        client->grid[grid_y][grid_x] = c;
    }
}

void render(TextClient* client, AirDefense* env) {
    clear_grid(client);
    
    // Draw aircraft
    set_char(client, env->aircraft.base.x, env->aircraft.base.y, 'A');
    
    // Draw seekers
    for(int i = 0; i < env->n_seekers; i++) {
        if(env->seekers[i].base.active) {
            set_char(client, env->seekers[i].base.x, env->seekers[i].base.y, 'M');
        }
    }
    
    // Draw flares
    for(int i = 0; i < env->n_flares; i++) {
        if(env->flares[i].base.active) {
            set_char(client, env->flares[i].base.x, env->flares[i].base.y, '*');
        }
    }
    
    // Print grid and status
    printf("\033[2J\033[H");  // Clear screen and move cursor to top
    printf("Aircraft Defense Simulation\n");
    printf("Flares: %d/%d  Score: %.1f\n", 
        env->aircraft.flares_remaining, 
        env->config.max_flares,
        env->aircraft.score);
    
    // Draw border and grid
    printf("┌");
    for(int x = 0; x < GRID_WIDTH; x++) printf("─");
    printf("┐\n");
    
    for(int y = 0; y < GRID_HEIGHT; y++) {
        printf("│");
        for(int x = 0; x < GRID_WIDTH; x++) {
            printf("%c", client->grid[y][x]);
        }
        printf("│\n");
    }
    
    printf("└");
    for(int x = 0; x < GRID_WIDTH; x++) printf("─");
    printf("┘\n");
    
    // Print legend
    printf("A=Aircraft  M=Missile  *=Flare\n");
    fflush(stdout);
}