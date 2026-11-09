#include "map.h"
#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  

#define BASE_GOLD_REWARD 10

void generate_map(char*** map_ptr, int* out_height, int* out_width) {
	int height = rand() % 26 + 5; // от 5 до 30
	int width = rand() % 26 + 5;
	*out_height = height;
	*out_width = width;

	char** map = (char**)malloc(height * sizeof(char*));
	for (int i = 0; i < height; i++) {
		map[i] = (char*)malloc(width * sizeof(char));
	}
	*map_ptr = map;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
				map[i][j] = '#';
			}
			else {
				int chance = rand() % 100;
				if (chance < 15) map[i][j] = 'T';
				else if (chance >= 15 && chance < 25) map[i][j] = 'R';
				else map[i][j] = '.';
			}
		}
	}

	// Находим случайную свободную ячейку '.' для лестницы вниз 
	while (1) {
		int d_y = rand() % (height - 2) + 1;
		int d_x = rand() % (width - 2) + 1;

		if (map[d_y][d_x] == '.') {
			map[d_y][d_x] = 'D'; // Ставим лестницу вниз
			// Прорубаем коридор от D влево
			for (int i = d_x; i > 0; i--) {
				if (map[d_y][i] == 'T' || map[d_y][i] == 'R') {
					map[d_y][i] = '.';
				}
			}
			break;
		}
	}
}

void print_map(char** map, int player_y, int player_x, int level, int height, int width) {
	system("cls"); //Очищаем экран перед каждым показом
	int radius = 4; // Радиус видимости для тумана
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Считаем расстояние от игрока до текущей клетки (теорема Пифагора)
			int dy = i - player_y;
			int dx = j - player_x;
			double dist = sqrt(dy * dy + dx * dx);

			// Если это 1-й этаж или клетка в радиусе видимости, показываем её
			if (level == 1 || dist <= radius) {
				printf("%c", map[i][j]);
			}
			else {
				printf("?"); // Иначе скрываем туманом
			}
		}
		printf("\n");
	}
}

void free_map(char** map, int height) {
	if (map == NULL) return; //Если карты в памяти и так нет — ничего делать не нужно
	for (int i = 0; i < height; i++) { //высота нужна чтобы компьютер знал, сколько именно строк ему придется удалить в цикле
		free(map[i]); //Освобождаем память, выделенную под каждую строку
	}
	free(map); //Освобождаем память от главного списка, где хранились адреса этих строк
}

void spawn_player(char** map, int height, int width, int level, int* out_y, int* out_x) {
	// Ищем свободную клетку для героя
	while (1) {
		int y = rand() % (height - 2) + 1; // + 1 — сдвигает координату вперёд
		int x = rand() % (width - 2) + 1;
		if (map[y][x] == '.') {
			map[y][x] = '@'; // Ставим героя
			*out_y = y; //Сохраняем координату Y игрока
			*out_x = x;
			break;
		}
	}

	// Если этаж не первый, ставим U (лестницу вверх) рядом с героем
	if (level > 1) {
		int dirs[4][2] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 } }; //проверяем 4 стороны
		for (int i = 0; i < 4; i++) {
			int ny = *out_y + dirs[i][0];
			int nx = *out_x + dirs[i][1];
			if (ny > 0 && ny < height - 1 && nx > 0 && nx < width - 1 && map[ny][nx] == '.') {
				map[ny][nx] = 'U'; //ставим лестницу в первую свободную клетку
				break;
			}
		}
	}
}

void start_adventure(Player* p, int* current_day, int* current_hour) {
	char** map = NULL; //Переменная для карты, пока пустая
	int height = 0; //Будущая высота карты
	int width = 0; //Будущая ширина карты
	int player_y = 0; //Строка, на которой стоит фермер 
	int player_x = 0; // Столбец, на котором стоит фермер
	int level = 1; //текущий этаж 

	generate_map(&map, &height, &width); //Вызываем генерацию карты и спавн игрока
	spawn_player(map, height, width, level, &player_y, &player_x);

	while (1) {
		char original_cell = map[player_y][player_x];
		map[player_y][player_x] = '@';
		print_map(map, player_y, player_x, level, height, width);

		// HUD (Интерфейс)
		printf("HP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n", p->hp, p->max_hp, p->stamina, p->max_stamina, p->gold, level, *current_hour);
		printf("=== Глубина: %d ===\n", level);
		printf("\nУправление: W-вверх, A-влево, S-вниз, D-вправо, Q-вернуться домой\n");

		// Возвращаем на место то, что было под игроком
		if (original_cell == '@') {
			map[player_y][player_x] = '.';
		}
		else {
			map[player_y][player_x] = original_cell;
		}

		printf("Ваш ход: ");
		char choice;
		if (scanf_s(" %c", &choice, 1) != 1) {
			clear_buffer();
			continue;
		}
		clear_buffer();

		if (choice == 'q' || choice == 'Q') { //Если нажали Q
			if (original_cell == 'U') {
				free_map(map, height);
				break;
			}
			else {
				printf("\nВыход домой возможен только на лестнице вверх (U)!\n");
				system("pause");
				continue;
			}
		}

		int next_y = player_y;
		int next_x = player_x;

		if (choice == 'w' || choice == 'W') next_y--; //Шаг вверх
		else if (choice == 's' || choice == 'S') next_y++; // Шаг вниз
		else if (choice == 'a' || choice == 'A') next_x--; //Шаг влево
		else if (choice == 'd' || choice == 'D') next_x++; // Шаг вправо

		if (next_y >= 0 && next_y < height && next_x >= 0 && next_x < width) {
			char target = map[next_y][next_x];
			if (target == '#' || target == 'T' || target == 'R') {
				printf("\nТам препятствие! Путь заблокирован\n");
				system("pause");
			}
			else if (target == 'D') { // Если наступили на лестницу вниз
				free_map(map, height);
				level++;
				generate_map(&map, &height, &width);
				spawn_player(map, height, width, level, &player_y, &player_x);

				p->stamina -= 5;
				if (p->stamina < 0) p->stamina = 0;

				*current_hour += 1;
				if (*current_hour >= 24) {
					*current_hour = 0;
					*current_day += 1;
				}
				// Награда за исследование
				int reward = BASE_GOLD_REWARD * level;
				p->gold += reward;
				printf("\nВы спустились на %d этаж! Награда: %d золота.\n", level, reward);
			}
			else {
				player_y = next_y;
				player_x = next_x;

				// Трата 1 выносливости за шаг
				p->stamina -= 1;
				if (p->stamina < 0) p->stamina = 0;
			}
		}
		// экстренная эвакуация 
		if (p->stamina <= 0 || (*current_hour == 2 && *current_day >= 1)) {
			if (p->stamina > 0 && *current_hour == 2) {
				printf("\nНаступило 02:00 ночи! Фермер падает без сил.\n");
			}
			else {
				printf("\nВы упали без сил! Спасательная служба доставляет вас домой.\n");
			}
			int lost_gold = p->gold / 2;
			p->gold -= lost_gold;
			printf("За услуги забрали %d золота. У вас осталось %d золота.\n", lost_gold, p->gold);

			free_map(map, height);
			system("pause");
			break;
		}
	}
}
