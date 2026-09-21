#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define INVENTORY_SIZE 10
#define ITEM_EMPTY 0
#define ITEM_DEREVO 1
#define ITEM_KAMEN 2
#define ITEM_SEMENA 3
#define ITEM_PSHENICA 4
#define ITEM_INSTRUMENT 5
#define ITEM_MORKOV 6
#define ITEM_VODA 7 
#define ITEM_UROZHAY 8
#define ITEM_MOLOKO 9
int main() {
	setlocale(0, ""); // чтобы буквы нормальные были 
	int current_day = 1;
	int current_hour = 8;
	int inventory[INVENTORY_SIZE] = { ITEM_EMPTY,ITEM_DEREVO,ITEM_KAMEN,ITEM_SEMENA,ITEM_PSHENICA,ITEM_INSTRUMENT,ITEM_MORKOV,ITEM_VODA,ITEM_UROZHAY,ITEM_MOLOKO };
	int user_choice;
	int add_hours;
	int slot_index;
	int item_id;
	int cleared_slots;
	int i;
	while (1) {  //бесконечный цикл 
		printf("0.Выход\n");
		printf("1.Посмотреть на часы\n");
		printf("2.Промотать время(Поработать)\n");
		printf("3.Посмотреть инвентарь\n");
		printf("4.Положить предметы в слот\n");
		printf("5.Выбросить предмет\n");
		printf("6.Очистка от мусора\n");
		if (scanf_s("%d", &user_choice) != 1) {
			printf("Неверный ввод\n");
			scanf_s("%*s");
			continue;
		}
		switch (user_choice) {
		case 0:
			return 0;
		case 1:
			printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
			break;
		case 2: {
			printf("Сколько часов поработать?: ");
			if (scanf_s("%d", &add_hours) != 1) {
				printf("Неверный ввод\n");
				scanf_s("%*s"); //пропускаем сохранение неверного ввода 
				continue;
			}
			if (add_hours < 0) {
				printf("Ошибка: нельзя отработать отрицательное количество времени\n");
				break;
			}
			current_hour = current_hour + add_hours; //добавление часов 
			current_day = current_day + (current_hour / 24); // считаем сколько полных дней содержится в накопившихся часах 
			current_hour = current_hour % 24; // оставлеяте остаток, который перешел на новые сутки
			printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
			break;
		}
		case 3:
			for (int i = 0; i < INVENTORY_SIZE; i++) {
				printf("Слот %d: [%d]", i, inventory[i]);
				switch (inventory[i]) {
				case ITEM_EMPTY: printf("(Пусто)\n"); break;
				case ITEM_DEREVO: printf("(Дерево)\n"); break;
				case ITEM_KAMEN: printf("(Камень)\n"); break;
				case ITEM_SEMENA: printf("(Семена)\n"); break;
				case ITEM_PSHENICA: printf("(Пшеница)\n"); break;
				case ITEM_INSTRUMENT: printf("(Инструмент)\n"); break;
				case ITEM_MORKOV: printf("(Морковь)\n"); break;
				case ITEM_VODA: printf("(Вода)\n"); break;
				case ITEM_UROZHAY: printf("(Урожай)\n"); break;
				case ITEM_MOLOKO: printf("(Молоко)\n"); break;
				}
			}
			break;
		case 4:
			printf("Введите номер слота(0-9):");
			if (scanf_s("%d", &slot_index) != 1) {
				printf("Неверный ввод\n");
				scanf_s("%*s");
				continue;
			}
			if (slot_index >= 0 && slot_index < INVENTORY_SIZE) {
				printf("Введите ID предмета(0-9):");
				if (scanf_s("%d", &item_id) != 1) {
					printf("Неверный ввод\n");
					scanf_s("%*s");
					continue;
				}
				if (item_id < 0 || item_id>9) {
					printf("ID предмета должен быть от 0 до 9!");
					break;
				}
				inventory[slot_index] = item_id;
				printf("Предмет изменен\n");
			}
			else {
				printf("Неверный номер слота\n");
				break;
			}
			break;
		case 5:
			system("cls");
			printf("Введите номер слота для удаления(0-9):");
			if (scanf_s("%d", &slot_index) != 1) {
				printf("Неверный ввод\n");
				scanf_s("%*s");
				continue;
			}
			if (slot_index >= 0 && slot_index < INVENTORY_SIZE) {
				inventory[slot_index] = ITEM_EMPTY;
				printf("Слот удален\n");
			}
			else {
				printf("Неверный номер слота\n");
			}
			break;
		case 6:
			printf("Выберете предмет для удаления(0-9):\n");
			if (scanf_s("%d", &item_id) != 1) {
				printf("Неверный ввод\n");
				scanf_s("%*s");
				continue;

			}
			cleared_slots = 0;
			for (i = 0; i < INVENTORY_SIZE; i++) {
				if (inventory[i] == item_id) {
					inventory[i] = ITEM_EMPTY;
					cleared_slots++;
				}
			}
			printf("Очистка завершена.Очещено слотов: %d\n", cleared_slots);
			break;
		default:
			system("cls"); //чистка строки
			printf("Неверный пункт ввода меню\n");
			scanf_s("%*s");
			continue; // возвращает в начало цикла в меню 		
		}
	}
}










