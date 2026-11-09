#include <stdio.h>
#include "utils.h"

void clear_buffer(void) { 
    int c;
    while ((c = getchar()) != '\n' && c != EOF); //î÷èñòêà 
}
void handle_time_menu(int* day, int* hour, Player* p) {
    int choice;
    int add_hours;
    while (1) {
        printf("\n=== ÓÏÐÀÂËÅÍÈÅ ÂÐÅÌÅÍÅÌ ===\n");
        printf("Òåêóùåå âðåìÿ: Äåíü %d, %02d:00\n", *day, *hour);
        printf("1. Ïîñìîòðåòü íà ÷àñû\n");
        printf("2. Ïîäîæäàòü (ïðîìîòàòü âðåìÿ)\n");
        printf("3. Ëå÷ü ñïàòü (äî óòðà)\n");
        printf("0. Íàçàä â ãëàâíîå ìåíþ\n");
        printf("Âàø âûáîð: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("Íåâåðíûé ââîä\n");
            clear_buffer();
            continue;
        }
        clear_buffer();

        if (choice == 0) break;

        if (choice == 1) {
            printf("Òåêóùåå âðåìÿ: Äåíü %d, %02d:00\n", *day, *hour);
        }
        else if (choice == 2) {
            //  íåëüçÿ æäàòü íî÷üþ 
            if (*hour >= 2 && *hour < 8) {
                printf("Íî÷üþ ôåðìåð äîëæåí ñïàòü! Íåëüçÿ æäàòü äî óòðà, èäèòå ñïàòü (ïóíêò 3).\n");
                continue;
            }
            printf("Ñêîëüêî ÷àñîâ ïîäîæäàòü?: ");
            if (scanf_s("%d", &add_hours) != 1) {
                printf("Íåâåðíûé ââîä\n");
                scanf_s("%*s");
                continue;
            }
            if (add_hours < 0) {
                printf("Îøèáêà: íåëüçÿ îòðàáîòàòü îòðèöàòåëüíîå êîëè÷åñòâî âðåìåíè\n");
                break;
            }
            *hour = *hour + add_hours;
            // Ïðîâåðÿåì, íå ïðîñêî÷èì ëè ãðàíèöó 02:00 íî÷è
            int total_hours = *hour + add_hours;
            int end_hour = total_hours % 24; // Âî ñêîëüêî ìû ïðîñíåìñÿ
            //ñ 2 äî 8 óòðà 
            if (*hour < 2 || *hour >= 8) {
                if (end_hour >= 2 && end_hour < 8 && total_hours < 24) {
                    printf("Íåëüçÿ ïðîìàòûâàòü âðåìÿ ÷åðåç 02:00 íî÷è! Ôåðìåð äîëæåí ñïàòü.\n");
                    continue;
                }
            }
            *day = *day + (*hour / 24);
            *hour = *hour % 24;
            printf("Òåêóùåå âðåìÿ: Äåíü %d, %02d:00\n", *day, *hour);
        }
        else if (choice == 3) {
            printf("Ôåðìåð ëåã ñïàòü\n");
            *day += 1;
            *hour = 8; // Ïðîñûïàåìñÿ â 8 óòðà
            if (p != NULL) {
                p->hp = p->max_hp;
                p->stamina = p->max_stamina;
                printf("Çäîðîâüå è âûíîñëèâîñòü âîññòàíîâëåíû!\n");
            }
            printf("Íàñòóïèëî óòðî: Äåíü %d, 08:00\n", *day);
        }
        else {
            printf("Íåâåðíûé ïóíêò ìåíþ\n");
        }
    }
}
int can_go_adventure(Player* p, int current_hour) { //ïðîâåðêà, ìîæåò ëè ãåðîé îòïðàâèòüñÿ â ïðèêëþ÷åíèå 
    if (p == NULL) {
        printf("Ñíà÷àëà ñîçäàéòå ïåðñîíàæà!\n");
        return 0;
    }
    if (p->stamina <= 0) {
        printf("Ó âàñ íåò ñèë! Âûíîñëèâîñòü íà íóëå. Èäèòå ñïàòü.\n");
        return 0;
    }
    if (current_hour >= 2 && current_hour < 8) {
        printf("Íà óëèöå íî÷ü (ïîñëå 02:00)! Ôåðìåð äîëæåí ñïàòü, à íå ãóëÿòü ïî ïîäçåìåëüÿì.\n");
        return 0;
    }
    return 1; // Âñå ïðîâåðêè ïðîéäåíû
}
