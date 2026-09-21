#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
int main() {
	setlocale(0, ""); // чтобы буквы нормальные были 
	int current_day = 1;
	int current_hour = 8;
	int inventory[10] = { 1,2,3,4,0,0,1,0,0,0 };
	int user_choice;
	int add_hours;
	int slot_index;
	int item_id;
	int cleared_slots;
	int i;
	while (1) {
		printf("0.Выход\n");
		printf("1.Посмотреть на часы\n");
		printf("2.Промотать время(Поработать)\n");
		printf("3.Посмотреть инвентарь\n");
		printf("4.Положить предметы в слот\n");
		printf("5.Выбросить предмет\n");
		printf("6.Очистка от мусора\n");
		if (scanf_s("%d", &user_choice) != 1) {
			printf("Неверный ввод\n");
			system("pause");
			return 0;
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
				system("pause");
				return 0;
			}
			current_hour = current_hour + add_hours; //добавление часов 
			current_day = current_day + (current_hour / 24); // считаем сколько полных дней содержится в накопившихся часах 
			current_hour = current_hour % 24; // оставлеяте остаток, который перешел на новые сутки
			printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
			break;
		}
		case 3:
			for (int i = 0; i < 10; i++) {
				printf("Слот %d: [%d]", i, inventory[i]);
				switch (inventory[i]) {
				case 0: printf("(Пусто)\n"); break;
				case 1: printf("(Дерево)\n"); break;
				case 2: printf("(Камень)\n"); break;
				case 3: printf("(Семена)\n"); break;
				case 4: printf("(Пшеница)\n"); break;
				case 5: printf("(Инструмент)\n"); break;
				case 6: printf("(Морковь)\n"); break;
				case 7: printf("(Вода)\n"); break;
				case 8: printf("(Урожай)\n"); break;
				case 9: printf("(Молоко)\n"); break;
				}
			}
			break;
		case 4:
			printf("Введите номер слота(0-9):");
			if (scanf_s("%d", &slot_index) != 1) {
				printf("Неверный ввод\n");
				system("pause");
				return 0;
			}
			if (slot_index >= 0 && slot_index < 10) {
				printf("Введите ID предмета(0-9):");
				if (scanf_s("%d", &item_id) != 1) {
					printf("Невернй ввод\n");
					system("pause");
					return 0;
				}
				inventory[slot_index] = item_id;
			}
			else {
				printf("Неверный номер слота\n");
			}

			printf("Предмет изменен\n");
			break;
		case 5:
			system("cls"); 
			printf("Введите номер слота для удаления(0-9):");
			if (scanf_s("%d", &slot_index) != 1) {
				printf("Неверный ввод\n");
				system("pause");
				return 0;
			}
			if (slot_index >= 0 && slot_index < 10) {
				inventory[slot_index] = 0;
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
				system("pause");
				return 0;
			}
			cleared_slots = 0; 
			for (i = 0; i < 10; i++) {
				if (inventory[i] == item_id) {
					inventory[i] = 0;
					cleared_slots++;
				}
			}
			printf("Очистка завершена.Очещено слотов: %d\n", cleared_slots);
			break;
		default: 
			system("cls"); //чистка строки
			printf("Неверный пункт ввода меню\n");
			scanf_s("%*s");
			break;
		}
	}
}










