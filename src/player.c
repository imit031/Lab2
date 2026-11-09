#define _CRT_SECURE_NO_WARNINGS //îòêëþ÷åíèå ïðåäóïðåæäåíèé áåçîïàñíîñòè(÷òîáû íå áûëî îøèáîê)
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "player.h"
#include "utils.h"  //  î÷èñòêà áóôåðà
void create_player(Player** p) { // ïðèíèìàåò óêàçàòåëü íà óêàçàòåëü íà ñòðóêòóðó Player
	*p = (Player*)malloc(sizeof(Player)); //âûäåëÿåì ïàìÿòü ïîä ñòðóêòóðó 
    if (*p == NULL) {
        printf("Îøèáêà: íå óäàëîñü âûäåëèòü ïàìÿòü ïîä èãðîêà\n");
        return;
    }
    printf("Ââåäèòå èìÿ ôåðìåðà: ");
    fgets((*p)->name, 32, stdin);
    // Óáèðàåì Enter èç èìåíè
    for (int i = 0; (*p)->name[i] != '\0'; i++) {
        if ((*p)->name[i] == '\n') {
            (*p)->name[i] = '\0';
            break;
        }
    } 
    if (strlen((*p)->name) == 0) {  //åñëè ïîëüçîâàòåëü íè÷åãî íå ââåë
        strcpy((*p)->name, "Áåçûìÿííûé");
    }
    (*p)->max_hp = 100;
    (*p)->hp = 100;
    (*p)->max_stamina = 50; 
    (*p)->stamina = 50;
    (*p)->gold = 0;
    int start_inv[INVENTORY_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // ID ïðåäìåòîâ
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        (*p)->inventory[i] = start_inv[i]; //êîïèðóåì èíâåíòàðü â ñòðóêòóðó 
    }
    // Èçíà÷àëüíî ýêèïèðîâêà ïóñòà
    for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
        strcpy((*p)->equipment[i].name, "Ïóñòî");
        (*p)->equipment[i].bonus_hp = 0;
        (*p)->equipment[i].bonus_stamina = 0;
        (*p)->equipment[i].slot = (EquipSlot)i;
    }
    printf("Ïåðñîíàæ %s óñïåøíî ñîçäàí!\n", (*p)->name);
}
void print_player_card(Player* p, int day, int hour, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    if (p == NULL) {
        printf("Îøèáêà: ïåðñîíàæ íå ñîçäàí!\n");
        return;
    }
    printf("\n=== ÊÀÐÒÎ×ÊÀ ÏÅÐÑÎÍÀÆÀ ===\n");
    printf("Èìÿ: %s\n", p->name);
    printf("Çäîðîâüå: %d/%d\n", p->hp, p->max_hp);
    printf("Âûíîñëèâîñòü: %d/%d\n", p->stamina, p->max_stamina);
    printf("Çîëîòî: %d\n", p->gold);
    printf("Òåêóùåå âðåìÿ: Äåíü %d, %02d:00\n", day, hour); // Äîáàâèëè âðåìÿ!

    printf("Èíâåíòàðü:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = p->inventory[i];
        printf("  Ñëîò %d: %s\n", i, item_names[id]); // Òåïåðü âûâîäèì íàçâàíèÿ!
    }
}
//ôóíêöèÿ óäàëåíèÿ èãðîêà 
void delete_player(Player** p) { //ïðèíèìàåò àäðåñ óêàçàòåëÿ íà èãðîêà 
    if (*p != NULL) {
        printf("Ïàìÿòü ñòðóêòóðû ïî àäðåñó %p îñâîáîæäåíà\n", (void*)*p); //%p-âûâîä àäðåñà ïàìÿòè 
        free(*p);
        *p = NULL;
    }
}
void handle_create_player(Player** p) {
    if (*p != NULL) { // Åñëè ïåðñîíàæ óæå åñòü
        printf("Ïåðñîíàæ óæå ñóùåñòâóåò. Òî÷íî óäàëèòü è ñîçäàòü íîâîãî? (1 - Äà, 0 - Íåò): ");
        int confirm;
        if (scanf_s("%d", &confirm) == 1 && confirm == 1) {
            delete_player(p);      // Óäàëÿåì ñòàðîãî
            clear_buffer();        // ×èñòèì áóôåð
            create_player(p);      // Ñîçäàåì íîâîãî
        }
        else {
            printf("Ñîçäàíèå îòìåíåíî.\n");
            clear_buffer();
        }
    }
    else { // Åñëè ïåðñîíàæà åùå íåò
        clear_buffer();
        create_player(p);
    }
}
void show_inventory(Player* p, char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    if (p == NULL) {
        printf("Ñíà÷àëà ñîçäàéòå ïåðñîíàæà!\n");
        return;
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = p->inventory[i];
        printf("Ñëîò %d: %s\n", i, item_names[id]);
    }
}
// Ôóíêöèÿ äëÿ ïåðåñ÷åòà ìàêñèìàëüíûõ õàðàêòåðèñòèê
void recalculate_stats(Player* p) {
    if (p == NULL) return; //åñëè èãðîêà íå ñóùåñòâóåò 
    // Ñáðàñûâàåì ê áàçîâûì çíà÷åíèÿì
    p->max_hp = 100;
    p->max_stamina = 50;
    for (int i = 0; i < EQUIPMENT_SLOTS; i++) { //öèêë ïî ñëîòàì ýêèïèðîâêè 
        p->max_hp += p->equipment[i].bonus_hp; //íà êàæäîì øàãå öèêëà ïðîãðàììà ñìîòðèò íà áîíóñû íàäåòîé âåùè è ïðèáàâëÿåò èõ ê òåêóùèì ìàêñèìóìàì
        p->max_stamina += p->equipment[i].bonus_stamina;
    }
    // ×òîáû òåêóùåå çäîðîâüå íå ïðåâûøàëî ìàêñèìàëüíîå
    if (p->hp > p->max_hp) p->hp = p->max_hp;
    if (p->stamina > p->max_stamina) p->stamina = p->max_stamina;
}

// Ôóíêöèÿ íàäåâàíèÿ âåùè
void equip_item(Player* p, Equipment item) { //ïðèíèìàåò èãðîêà è âåùü 
    if (p == NULL) return;
    if (item.slot >= 0 && item.slot < EQUIPMENT_SLOTS) { //ïðîâåðêà åñòü ëè òàêîé ñëîò 
        // Íàäåâàåì âåùü â íóæíûé ñëîò
        p->equipment[item.slot] = item;
        printf("Âû íàäåëè: %s\n", item.name);
        // Ïåðåñ÷èòûâàåì õàðàêòåðèñòèêè
        recalculate_stats(p); //âûçîâ ôóíêöèè ÷òîáû ,÷òîáû ïåðåñ÷èòàòü çäîðîâüå è âûíîñëèâîñòü ïîñëå òîãî, êàê íàäåëè âåùü
        printf("Íîâûå õàðàêòåðèñòèêè: HP %d/%d | Âûíîñëèâîñòü %d/%d\n", p->hp, p->max_hp, p->stamina, p->max_stamina);
    }
    else {
        printf("Îøèáêà: íåâåðíûé ñëîò ýêèïèðîâêè!\n");
    }
}
void handle_equipment_menu(Player* p) { //ôóíêöèÿ óïðàâëåíèÿ ýêèïèðîâêîé, ïðèíèìàåò óêàçàòåëü íà èãðîêà 
    if (p == NULL) {
        printf("Ñíà÷àëà ñîçäàéòå ïåðñîíàæà!\n");
        return;
    }

    int choice; //âûáîð îãðîêà 
    while (1) {
        printf("\n=== ÓÏÐÀÂËÅÍÈÅ ÑÍÀÐßÆÅÍÈÅÌ ===\n");
        printf("Òåêóùàÿ ýêèïèðîâêà:\n");
        printf("  Ãîëîâà: %s (HP +%d, Stamina +%d)\n", p->equipment[SLOT_HEAD].name, p->equipment[SLOT_HEAD].bonus_hp, p->equipment[SLOT_HEAD].bonus_stamina); //÷òî íàäåîòî íà ãîëîâó, áîíóñ ê ñèëå è âûíîñëèâîñòè 
        printf("  Òåëî:   %s (HP +%d, Stamina +%d)\n", p->equipment[SLOT_BODY].name, p->equipment[SLOT_BODY].bonus_hp, p->equipment[SLOT_BODY].bonus_stamina);//
        printf("  Îáóâü:  %s (HP +%d, Stamina +%d)\n", p->equipment[SLOT_FEET].name, p->equipment[SLOT_FEET].bonus_hp, p->equipment[SLOT_FEET].bonus_stamina);

        printf("\n1. Íàäåòü Æåëåçíûé øëåì (Ãîëîâà, +20 HP)\n");
        printf("2. Íàäåòü Êîæàíóþ áðîíþ (Òåëî, +10 HP, +15 Stamina)\n");
        printf("3. Íàäåòü Ñàïîãè ñêîðîñòè (Îáóâü, +10 Stamina)\n");
        printf("0. Ñíÿòü âñþ ýêèïèðîâêó\n");
        printf("-1. Íàçàä â ãëàâíîå ìåíþ\n");
        printf("Âàø âûáîð: ");
        if (scanf_s("%d", &choice) != 1) {
            printf("Íåâåðíûé ââîä\n");
            scanf_s("%*s");
            continue;
        }

        if (choice == -1) break;

        if (choice == 1) {
            Equipment helmet = { "Æåëåçíûé øëåì", SLOT_HEAD, 20, 0 };
            equip_item(p, helmet);
        }
        else if (choice == 2) {
            Equipment armor = { "Êîæàíàÿ áðîíÿ", SLOT_BODY, 10, 15 };
            equip_item(p, armor);
        }
        else if (choice == 3) {
            Equipment boots = { "Ñàïîãè ñêîðîñòè", SLOT_FEET, 0, 10 };
            equip_item(p, boots);
        }
        else if (choice == 0) {
            // Ñíèìàåì âñ¸
            for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
                strcpy(p->equipment[i].name, "Ïóñòî");
                p->equipment[i].bonus_hp = 0;
                p->equipment[i].bonus_stamina = 0;
            }
            recalculate_stats(p);
            printf("Âñÿ ýêèïèðîâêà ñíÿòà. Õàðàêòåðèñòèêè âåðíóëèñü ê áàçîâûì.\n");
        }
        else {
            printf("Íåâåðíûé ïóíêò ìåíþ\n");
        }
    }
}
