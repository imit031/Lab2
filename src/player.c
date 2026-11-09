#define _CRT_SECURE_NO_WARNINGS //отключение предупреждений безопасности(чтобы не было ошибок)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "utils.h"  //  очистка буфера
void create_player(Player** p) { // принимает указатель на указатель на структуру Player
	*p = (Player*)malloc(sizeof(Player)); //выделяем память под структуру 
    if (*p == NULL) {
        printf("Ошибка: не удалось выделить память под игрока\n");
        return;
    }
    printf("Введите имя фермера: ");
    fgets((*p)->name, 32, stdin);
    // Убираем Enter из имени
    for (int i = 0; (*p)->name[i] != '\0'; i++) {
        if ((*p)->name[i] == '\n') {
            (*p)->name[i] = '\0';
            break;
        }
    } 
    if (strlen((*p)->name) == 0) {  //если пользователь ничего не ввел
        strcpy((*p)->name, "Безымянный");
    }
    (*p)->max_hp = 100;
    (*p)->hp = 100;
    (*p)->max_stamina = 50; 
    (*p)->stamina = 50;
    (*p)->gold = 0;
    int start_inv[INVENTORY_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // ID предметов
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        (*p)->inventory[i] = start_inv[i]; //копируем инвентарь в структуру 
    }
    // Изначально экипировка пуста
    for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
        strcpy((*p)->equipment[i].name, "Пусто");
        (*p)->equipment[i].bonus_hp = 0;
        (*p)->equipment[i].bonus_stamina = 0;
        (*p)->equipment[i].slot = (EquipSlot)i;
    }
    printf("Персонаж %s успешно создан!\n", (*p)->name);
}
void print_player_card(Player* p, int day, int hour, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    if (p == NULL) {
        printf("Ошибка: персонаж не создан!\n");
        return;
    }
    printf("\n=== КАРТОЧКА ПЕРСОНАЖА ===\n");
    printf("Имя: %s\n", p->name);
    printf("Здоровье: %d/%d\n", p->hp, p->max_hp);
    printf("Выносливость: %d/%d\n", p->stamina, p->max_stamina);
    printf("Золото: %d\n", p->gold);
    printf("Текущее время: День %d, %02d:00\n", day, hour); // Добавили время!

    printf("Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = p->inventory[i];
        printf("  Слот %d: %s\n", i, item_names[id]); // Теперь выводим названия!
    }
}
//функция удаления игрока 
void delete_player(Player** p) { //принимает адрес указателя на игрока 
    if (*p != NULL) {
        printf("Память структуры по адресу %p освобождена\n", (void*)*p); //%p-вывод адреса памяти 
        free(*p);
        *p = NULL;
    }
}
void handle_create_player(Player** p) {
    if (*p != NULL) { // Если персонаж уже есть
        printf("Персонаж уже существует. Точно удалить и создать нового? (1 - Да, 0 - Нет): ");
        int confirm;
        if (scanf_s("%d", &confirm) == 1 && confirm == 1) {
            delete_player(p);      // Удаляем старого
            clear_buffer();        // Чистим буфер
            create_player(p);      // Создаем нового
        }
        else {
            printf("Создание отменено.\n");
            clear_buffer();
        }
    }
    else { // Если персонажа еще нет
        clear_buffer();
        create_player(p);
    }
}
void show_inventory(Player* p, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    if (p == NULL) {
        printf("Сначала создайте персонажа!\n");
        return;
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = p->inventory[i];
        printf("Слот %d: %s\n", i, item_names[id]);
    }
}
// Функция для пересчета максимальных характеристик
void recalculate_stats(Player* p) {
    if (p == NULL) return; //если игрока не существует 
    // Сбрасываем к базовым значениям
    p->max_hp = 100;
    p->max_stamina = 50;
    for (int i = 0; i < EQUIPMENT_SLOTS; i++) { //цикл по слотам экипировки 
        p->max_hp += p->equipment[i].bonus_hp; //на каждом шаге цикла программа смотрит на бонусы надетой вещи и прибавляет их к текущим максимумам
        p->max_stamina += p->equipment[i].bonus_stamina;
    }
    // Чтобы текущее здоровье не превышало максимальное
    if (p->hp > p->max_hp) p->hp = p->max_hp;
    if (p->stamina > p->max_stamina) p->stamina = p->max_stamina;
}

// Функция надевания вещи
void equip_item(Player* p, Equipment item) { //принимает игрока и вещь 
    if (p == NULL) return;
    if (item.slot >= 0 && item.slot < EQUIPMENT_SLOTS) { //проверка есть ли такой слот 
        // Надеваем вещь в нужный слот
        p->equipment[item.slot] = item;
        printf("Вы надели: %s\n", item.name);
        // Пересчитываем характеристики
        recalculate_stats(p); //вызов функции чтобы ,чтобы пересчитать здоровье и выносливость после того, как надели вещь
        printf("Новые характеристики: HP %d/%d | Выносливость %d/%d\n", p->hp, p->max_hp, p->stamina, p->max_stamina);
    }
    else {
        printf("Ошибка: неверный слот экипировки!\n");
    }
}
void handle_equipment_menu(Player* p) { //функция управления экипировкой, принимает указатель на игрока 
    if (p == NULL) {
        printf("Сначала создайте персонажа!\n");
        return;
    }

    int choice; //выбор огрока 
    while (1) {
        printf("\n=== УПРАВЛЕНИЕ СНАРЯЖЕНИЕМ ===\n");
        printf("Текущая экипировка:\n");
        printf("  Голова: %s (HP +%d, Stamina +%d)\n", p->equipment[SLOT_HEAD].name, p->equipment[SLOT_HEAD].bonus_hp, p->equipment[SLOT_HEAD].bonus_stamina); //что надеото на голову, бонус к силе и выносливости 
        printf("  Тело:   %s (HP +%d, Stamina +%d)\n", p->equipment[SLOT_BODY].name, p->equipment[SLOT_BODY].bonus_hp, p->equipment[SLOT_BODY].bonus_stamina);//
        printf("  Обувь:  %s (HP +%d, Stamina +%d)\n", p->equipment[SLOT_FEET].name, p->equipment[SLOT_FEET].bonus_hp, p->equipment[SLOT_FEET].bonus_stamina);

        printf("\n1. Надеть Железный шлем (Голова, +20 HP)\n");
        printf("2. Надеть Кожаную броню (Тело, +10 HP, +15 Stamina)\n");
        printf("3. Надеть Сапоги скорости (Обувь, +10 Stamina)\n");
        printf("0. Снять всю экипировку\n");
        printf("-1. Назад в главное меню\n");
        printf("Ваш выбор: ");
        if (scanf_s("%d", &choice) != 1) {
            printf("Неверный ввод\n");
            scanf_s("%*s");
            continue;
        }

        if (choice == -1) break;

        if (choice == 1) {
            Equipment helmet = { "Железный шлем", SLOT_HEAD, 20, 0 };
            equip_item(p, helmet);
        }
        else if (choice == 2) {
            Equipment armor = { "Кожаная броня", SLOT_BODY, 10, 15 };
            equip_item(p, armor);
        }
        else if (choice == 3) {
            Equipment boots = { "Сапоги скорости", SLOT_FEET, 0, 10 };
            equip_item(p, boots);
        }
        else if (choice == 0) {
            // Снимаем всё
            for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
                strcpy(p->equipment[i].name, "Пусто");
                p->equipment[i].bonus_hp = 0;
                p->equipment[i].bonus_stamina = 0;
            }
            recalculate_stats(p);
            printf("Вся экипировка снята. Характеристики вернулись к базовым.\n");
        }
        else {
            printf("Неверный пункт меню\n");
        }
    }
}