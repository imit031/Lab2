#ifndef PLAYER_H
#define PLAYER_H

#define INVENTORY_SIZE 10
#define EQUIPMENT_SLOTS 3
// Перечисление для типов слотов(голова, тело, обувь)
typedef enum {
    SLOT_HEAD = 0,
    SLOT_BODY = 1,
    SLOT_FEET = 2
} EquipSlot; //имя перечисления 
typedef struct {
    char name[32];      // Название предмета
    EquipSlot slot;     // Тип слота
    int bonus_hp;       // Бонус к здоровью
    int bonus_stamina;  // Бонус к выносливости
} Equipment;

// Структура игрока
typedef struct {
    char name[32];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x;
    int y;
    int inventory[INVENTORY_SIZE];
    Equipment equipment[EQUIPMENT_SLOTS];
} Player;

void handle_create_player(Player** p);
void print_player_card(Player* p, int day, int hour, char item_names[10][32]);
void handle_equipment_menu(Player* p);
void equip_item(Player* p, Equipment item);
void recalculate_stats(Player* p);

typedef enum {
    MENU_EXIT = 0,
    MENU_CREATE_PLAYER = 1,
    MENU_PLAYER_CARD = 2,
    MENU_TIME = 3,
    MENU_INVENTORY = 4,
    MENU_PUT_ITEM = 5,
    MENU_DROP_ITEM = 6,
    MENU_CLEAR_ITEMS = 7,
    MENU_SEARCH = 8,
    MENU_DIARY = 9,
    MENU_CRAFT = 10,
    MENU_ADVENTURE = 11
} MenuOption;

#endif