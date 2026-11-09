#ifndef MAP_H
#define MAP_H

#include "player.h" // Чтобы map.c знал, кто такой Player
#define BASE_GOLD_REWARD 10
typedef struct {
    char** grid;      // Указатель на двумерный массив(карта)
    int width;        // Ширина
    int height;       // Высота
    int level;        // Счётчик этажей
} Map;

// Объявления функций для работы с картой
void generate_map(char*** map_ptr, int* out_height, int* out_width);
void print_map(char** map, int player_y, int player_x, int level, int height, int width);
void free_map(char** map, int height);
void spawn_player(char** map, int height, int width, int level, int* out_y, int* out_x);
void start_adventure(Player* p, int* current_day, int* current_hour);

#endif