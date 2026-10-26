#define _CRT_SECURE_NO_WARNINGS //чтобы не было ошибок при сипользовании fopen, strcpy
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h> 
#include <math.h> // Добавили для функции sqrt (туман войны)


#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
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

void clear_buffer(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void get_farmer_name(char* name) {
	printf("Введите имя фермера: ");
	fgets(name, 32, stdin);
	for (int i = 0; name[i] != '\0';i++) {
		if (name[i] == '\n') {
			name[i] = '\0';
			break;
		}
	}
}

void search_item(char item_names[MAX_ITEMS][MAX_NAME_LEN], int inventory[INVENTORY_SIZE]) {
	char search_name[MAX_NAME_LEN]; //переменная для хранения введенного слова 
	printf("Введите названия предмета для поиска: ");
	if (scanf_s("%31s", search_name, (unsigned)_countof(search_name)) != 1) { //возвращает 1, если  удалось успешно прочитать ровно одно слово
		while (getchar() != '\n'); //Если ввод сломался, чистим буфер клавиатуры
		return;  //досрочно выходим из функции
	}
	while (getchar() != '\n');
	search_name[MAX_NAME_LEN - 1] = '\0';  //квадратных скобках вычисляем индекс самого последнего элемента массива

	int target_id = -1; //Переменная для хранения найденного IDка не найдено)
	for (int i = 0; i < MAX_ITEMS; i++) { //Цикл обхода всего каталога предметов
		if (strcmp(item_names[i], search_name) == 0) {
			target_id = i; //запоминаем числовой ID этого предмета
			break;
		}
	}

	if (target_id == -1) {  //Если после цикла target_id остался равен -1, значит предмета нет в игре
		printf("Такого предмета нет в каталоге!\n");
		return;
	}

	int count = 0;  //Счетчик, сколько раз мы нашли предмет в рюкзаке
	printf("Предмет найден в слотах: ");
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		if (inventory[i] == target_id) {
			printf("%d ", i);
			count++;  //Увеличиваем счетчик найденных предметов
		}
	}
	printf("\n");
}

//запись состояния в дневник фермера 
void write_diary(int day, int hour, const char* name, char names[MAX_ITEMS][MAX_NAME_LEN], int inv[INVENTORY_SIZE]) { //Функция записи состояния принимает время, имя фермера, каталог названий и рюкзак9
	FILE* diary = fopen("diary.txt", "a");
	if (diary == NULL) { ////Защита,если файл не удалось открыть или создать
		printf("Ошибка: Не удалось открыть или создать файл diary.txt!\n");
		return;
	}
	fprintf(diary, "[День %d, %02d:00] Фермер %s сохранил состояние.\n", day, hour, name);
	for (int i = 0; i < INVENTORY_SIZE; i++) {  //Цикл для поочередного обхода всех 10 слотов инвентаря фермера
		int id = inv[i]; // Достаем числовой ID предмета из текущего слота рюкзака
		fprintf(diary, " Слот %d: %s\n", i, names[id]);
	}
	fclose(diary);
	printf("[Успешно]: Состояние фермы записано в diary.txt.\n");
}

//Проверка рецептов крафта 
void check_craft(int inv[INVENTORY_SIZE], char names[MAX_ITEMS][MAX_NAME_LEN]) {  //Функция проверки рецептов крафта принимает рюкзак фермера и каталог названий
	printf("--- ПРОВЕРКА РЕЦЕПТОВ КРАФТА ---\n");

	// 1. Считаем сколько ресурсов в рюкзаке
	int resources[MAX_ITEMS] = { 0 };  //Создаем и чистим память от мусора для подсчета количества наших ресурсов
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		resources[inv[i]]++;  //++ увеличивает значение в соответствующей ячейке на единицу
	}

	// 2. Открываем файлы
	FILE* in = fopen("input.txt", "r");
	FILE* out = fopen("output.txt", "w");

	if (in == NULL || out == NULL) {   //Защита: если хотя бы один файл не открылся, аварийно завершаем работу
		printf("Ошибка: не удалось открыть файлы!\n");
		if (in) fclose(in);
		if (out) fclose(out);
		return;
	}

	char craft_name[MAX_NAME_LEN];
	int req1, req2, req3;  //Переменные для хранения трех числовых ID компонентов рецепта

	while (fscanf(in, "%31s %d %d %d", craft_name, &req1, &req2, &req3) == 4) {
		int temp_res[MAX_ITEMS];
		for (int j = 0; j < MAX_ITEMS; j++) {
			temp_res[j] = resources[j]; // Копируем наши ресурсы во временный массив, чтобы не испортить реальный рюкзак
		}

		int can_craft = 1; //1-ресурсов хватает 
		if (temp_res[req1] > 0) {  //если первый ресурс есть в наличии 
			temp_res[req1]--; //забираем одну штуку 
		}
		else {
			can_craft = 0; // иначе, не хватает 
		}

		if (temp_res[req2] > 0) {
			temp_res[req2]--;
		}
		else {
			can_craft = 0;
		}

		if (temp_res[req3] > 0) {
			temp_res[req3]--;
		}
		else {
			can_craft = 0;
		}
		//Если после всех проверок флаг остался равен единице
		if (can_craft == 1) {
			printf("Рецепт %s: ДОСТУПНО\n", craft_name);
			fprintf(out, "Рецепт %s: ДОСТУПНО\n", craft_name);
		}
		else {
			printf("Рецепт %s: НЕ ХВАТАЕТ РЕСУРСОВ ( ", craft_name);
			fprintf(out, "Рецепт %s: НЕ ХВАТАЕТ РЕСУРСОВ ( ", craft_name);

			if (resources[req1] < 1 || (req1 == req2 && resources[req1] < 2) || (req1 == req2 && req1 == req3 && resources[req1] < 3)) { printf("%s ", names[req1]); fprintf(out, "%s ", names[req1]); } //проверяет три сценария нехватки ресурса
			if (resources[req2] < 1 || (req2 == req1 && resources[req2] < 2) || (req2 == req1 && req2 == req3 && resources[req2] < 3)) { printf("%s ", names[req2]); fprintf(out, "%s ", names[req2]); }
			if (resources[req3] < 1 || (req3 == req1 && resources[req3] < 2) || (req3 == req1 && req3 == req2 && resources[req3] < 3)) { printf("%s ", names[req3]); fprintf(out, "%s ", names[req3]); }

			printf(")\n");
			fprintf(out, ")\n");
		}
	}

	fclose(in);
	fclose(out);
	printf("[Успешно]: Результаты проверки записаны в output.txt.\n");

}

void load_item_catalogue(char names[MAX_ITEMS][MAX_NAME_LEN]) { //Функция загрузки каталога принимает массив строк для заполнения именами предметов
	FILE* file = fopen("items.txt", "r");
	if (file == NULL) {   //Защита: если файл items.txt отсутствует на диске
		printf("[Предупреждение]: Файл items.txt не найден!Загружен каталог по умолчанию.\n");
		strcpy(names[0], "Пусто");
		strcpy(names[1], "Дерево");
		strcpy(names[2], "Камень");
		strcpy(names[3], "Семена");
		strcpy(names[4], "Пшеница");
		strcpy(names[5], "Инструмент");
		strcpy(names[6], "Морковь");
		strcpy(names[7], "Вода");
		strcpy(names[8], "Урожай");
		strcpy(names[9], "Молоко");
		return;
	}
	int id; //еременная для временного хранения считанного из файла номера предмета
	char buffer[MAX_NAME_LEN] = { 0 };
	while (fscanf(file, "%d %31s", &id, buffer) == 2) {  // читаем файл, пока fscanf успешно находит пару из числа и слова 
		if (id >= 0 && id < MAX_ITEMS) {
			strcpy(names[id], buffer);  //берем считанное слово из buffer и записываем под номером id в таблицу 
		}
	}
	fclose(file);
	printf("[Успешно]: Каталог предметов загружен из файла items.txt.\n");

}

// ================= НОВЫЕ ФУНКЦИИ ДЛЯ 4 ЛАБЫ =================

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
		int y = rand() % (height - 2) + 1; // + 1 — сдвигает координату вперёд, чтобы фермер не заспавнился внутри стены
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
		int dirs[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }; //проверяем 4 стороны вокруг себя
		for (int i = 0; i < 4; i++) {
			int ny = *out_y + dirs[i][0];
			int nx = *out_x + dirs[i][1];
			if (ny > 0 && ny < height - 1 && nx > 0 && nx < width - 1 && map[ny][nx] == '.') {
				map[ny][nx] = 'U'; //ставим лестницу в первую свободную клетку из 4
				break;
			}
		}
	}
}

void generate_map(char*** map_ptr, int* out_height, int* out_width) {
	int height = rand() % 26 + 5; //26 возможных вариантов(от 5 до 30), 5-минимальная высота 
	int width = rand() % 26 + 5;
	*out_height = height; // Записываем по адресу высоту 
	*out_width = width;
	char** map = (char**)malloc(height * sizeof(char*)); //Просим память под список адресов строк карты
	for (int i = 0; i < height; i++) { //Запускаем цикл, чтобы выделить память отдельно под каждую строку карты
		map[i] = (char*)malloc(width * sizeof(char)); //Просим память под символы (ширину) для текущей строки под номером i
	}
	*map_ptr = map; //отдаем адрес в главную программу 

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) { //идет по клеткам строки слева направо (горизонталь)
			if (i == 0 || i == height - 1 || j == 0 || j == width - 1) { //если мы находимся на самом краю (на границе) карты
				map[i][j] = '#';  //Записываем в эту крайнюю клетку символ непроходимой стены
			}
			else {
				int chance = rand() % 100; //Получаем случайное число от 0 до 99
				if (chance < 15) {
					map[i][j] = 'T'; //15% шанс, что выпадет дерево
				}
				else if (chance >= 15 && chance < 25) {
					map[i][j] = 'R';  //10% шанс, что выпадет камень(числа 15 - 24)
				}
				else {
					map[i][j] = '.';
				}
			}
		}
	}

	// Находим случайную СВОБОДНУЮ ячейку '.' для лестницы вниз 'D'
	while (1) {
		int d_y = rand() % (height - 2) + 1;
		int d_x = rand() % (width - 2) + 1;

		if (map[d_y][d_x] == '.') {
			map[d_y][d_x] = 'D'; // Ставим лестницу вниз
			// Прорубаем коридор от D влево, чтобы игрок мог дойти
			for (int i = d_x; i > 0; i--) {
				if (map[d_y][i] == 'T' || map[d_y][i] == 'R') {
					map[d_y][i] = '.'; // Уничтожаем дерево или камень
				}
			}
			break; // Успешно поставили, выходим из цикла
		}
	}
}

void start_adventure() {
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

		if (choice == 'q' || choice == 'Q') { //Если нажали Q - проверяем, стоим ли на лестнице вверх
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

		if (choice == 'w' || choice == 'W') next_y--; //Шаг вверх (минус строка)
		else if (choice == 's' || choice == 'S') next_y++; // Шаг вниз (плюс строка) 
		else if (choice == 'a' || choice == 'A') next_x--; //Шаг влево (минус столбец)
		else if (choice == 'd' || choice == 'D') next_x++; // Шаг вправо (плюс столбец)

		if (next_y >= 0 && next_y < height && next_x >= 0 && next_x < width) {
			char target = map[next_y][next_x];
			if (target == '#' || target == 'T' || target == 'R') {
				printf("\nТам препятствие! Путь заблокирован\n");
				system("pause");
			}
			else if (target == 'D') { // Если наступили на лестницу вниз
				
				free_map(map, height); // Удаляем старую карту из памяти
				level++; // Увеличиваем счетчик глубины
				generate_map(&map, &height, &width); // Генерируем новую карту
				spawn_player(map, height, width, level, &player_y, &player_x); // Ставим героя
				
			}
			else {
				player_y = next_y;
				player_x = next_x;
			}
		}
	}
}



int main() {
	setlocale(LC_CTYPE, "rus"); // чтобы буквы нормальные были 
	system("chcp 1251 > nul"); // чтобы буквы нормальные были 
	srand(time(NULL)); 
	char farmer_name[32] = "фермер";
	char item_names[MAX_ITEMS][MAX_NAME_LEN];
	load_item_catalogue(item_names);

	get_farmer_name(farmer_name);
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
		printf("\n===Привет,%s! Добро пожаловать на ферму!===\n", farmer_name);
		printf("0.Выход\n");
		printf("1.Посмотреть на часы\n");
		printf("2.Промотать время(Поработать)\n");
		printf("3.Посмотреть инвентарь\n");
		printf("4.Положить предметы в слот\n");
		printf("5.Выбросить предмет\n");
		printf("6.Очистка от мусора\n");
		printf("7.Поиск предмета по названию\n");
		printf("8.Записать состояние в дневник фермера (diary.txt)\n");
		printf("9.Проверить рецепт крафта.(input.txt)\n");
		printf("10.Отправиться в приключение\n"); 

		if (scanf_s("%d", &user_choice) != 1) {
			printf("Неверный ввод\n");
			clear_buffer();
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
			for (int i = 0; i < INVENTORY_SIZE;i++) {  // i-номер слота 
				int id = inventory[i];  //достаем ID и сохраняем в перемененную id 
				printf("Слот %d: %s\n", i, item_names[id]);  //%d заменяется на номер слота(i), %s-на слово 
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
		case 7: {
			search_item(item_names, inventory);
			break;
		}

		case 8: {
			write_diary(current_day, current_hour, farmer_name, item_names, inventory);
			break;
		}

		case 9: {
			check_craft(inventory, item_names);
			break;
		}
		case 10:
			start_adventure();
			break;
		default:
			system("cls"); //чистка строки
			printf("Неверный пункт ввода меню\n");
			scanf_s("%*s");
			continue; // возвращает в начало цикла в меню 		
		}
	}
}









