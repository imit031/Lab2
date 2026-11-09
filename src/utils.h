#ifndef UTILS_H //// Защита от повторного включения
#define UTILS_H
#include "player.h"
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
void clear_buffer(void);
void handle_time_menu(int* day, int* hour, Player* p);
int can_go_adventure(Player* p, int current_hour);
#endif
