#include <stdio.h>
#include "utils.h"

void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); //очистка 
}
void handle_time_menu(int* day, int* hour, Player* p) {
    int choice;
    int add_hours;
    while (1) {
        printf("\n=== УПРАВЛЕНИЕ ВРЕМЕНЕМ ===\n");
        printf("Текущее время: День %d, %02d:00\n", *day, *hour);
        printf("1. Посмотреть на часы\n");
        printf("2. Подождать (промотать время)\n");
        printf("3. Лечь спать (до утра)\n");
        printf("0. Назад в главное меню\n");
        printf("Ваш выбор: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("Неверный ввод\n");
            clear_buffer();
            continue;
        }
        clear_buffer();

        if (choice == 0) break;

        if (choice == 1) {
            printf("Текущее время: День %d, %02d:00\n", *day, *hour);
        }
        else if (choice == 2) {
            //  нельзя ждать ночью 
            if (*hour >= 2 && *hour < 8) {
                printf("Ночью фермер должен спать! Нельзя ждать до утра, идите спать (пункт 3).\n");
                continue;
            }
            printf("Сколько часов подождать?: ");
            if (scanf_s("%d", &add_hours) != 1) {
                printf("Неверный ввод\n");
                scanf_s("%*s");
                continue;
            }
            if (add_hours < 0) {
                printf("Ошибка: нельзя отработать отрицательное количество времени\n");
                break;
            }
            *hour = *hour + add_hours;
            // Проверяем, не проскочим ли границу 02:00 ночи
            int total_hours = *hour + add_hours;
            int end_hour = total_hours % 24; // Во сколько мы проснемся
            //с 2 до 8 утра 
            if (*hour < 2 || *hour >= 8) {
                if (end_hour >= 2 && end_hour < 8 && total_hours < 24) {
                    printf("Нельзя проматывать время через 02:00 ночи! Фермер должен спать.\n");
                    continue;
                }
            }
            *day = *day + (*hour / 24);
            *hour = *hour % 24;
            printf("Текущее время: День %d, %02d:00\n", *day, *hour);
        }
        else if (choice == 3) {
            printf("Фермер лег спать\n");
            *day += 1;
            *hour = 8; // Просыпаемся в 8 утра
            if (p != NULL) {
                p->hp = p->max_hp;
                p->stamina = p->max_stamina;
                printf("Здоровье и выносливость восстановлены!\n");
            }
            printf("Наступило утро: День %d, 08:00\n", *day);
        }
        else {
            printf("Неверный пункт меню\n");
        }
    }
}
int can_go_adventure(Player* p, int current_hour) { //проверка, может ли герой отправиться в приключение 
    if (p == NULL) {
        printf("Сначала создайте персонажа!\n");
        return 0;
    }
    if (p->stamina <= 0) {
        printf("У вас нет сил! Выносливость на нуле. Идите спать.\n");
        return 0;
    }
    if (current_hour >= 2 && current_hour < 8) {
        printf("На улице ночь (после 02:00)! Фермер должен спать, а не гулять по подземельям.\n");
        return 0;
    }
    return 1; // Все проверки пройдены
}