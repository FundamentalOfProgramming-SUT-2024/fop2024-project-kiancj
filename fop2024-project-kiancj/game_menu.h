#ifndef game_menu_h
#define game_menu_h

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "map.h"
#define EASY_ITEMS 5
#define MEDIUM_ITEMS 10
#define HARD_ITEMS 15
#define EASY_TIME 40
#define MEDIUM_TIME 30
#define HARD_TIME 20



int user_c = 0;
User users[100];

void read_file_users(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%s %s %s %d %d %d %d", users[user_c].username, users[user_c].password, users[user_c].email, &users[user_c].score, &users[user_c].gold, &users[user_c].games, &users[user_c].playtime) != EOF) {
        user_c++;
    }

    fclose(file);
}

#define MENU_ITEMS 6
char *menu[] = {
    "1. New Game",
    "2. Continue Previous Game",
    "3. Score Board",
    "4. Settings",
    "5. Profile",
    "6. Exit"
};

int compare(const void *a, const void *b) {
    User *playerA = (User *)a;
    User *playerB = (User *)b;
    return playerB->score - playerA->score; 
}

void new_game();
void load_game();
void show_scores();
void settings();
void profile();
void exit_program();
void draw_border();

int main_game_menu() {
    read_file_users("users.txt");
    initscr();            
    cbreak();             
    noecho();             
    curs_set(0);          
    keypad(stdscr, TRUE); 
    int choice = 0;
    int highlight = 0;

    while (1) {
        clear();
        mvprintw(1, 2, "Game Menu");
        for (int i = 0; i < MENU_ITEMS; i++) {
            if (i == highlight)
                attron(A_REVERSE); 
            mvprintw(3 + i, 4, "%s", menu[i]);
            attroff(A_REVERSE);
        }
        int input = getch();
        switch (input) {
            case KEY_UP:
                highlight = (highlight == 0) ? MENU_ITEMS - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == MENU_ITEMS - 1) ? 0 : highlight + 1;
                break;
            case 10: 
                switch (highlight) {
                    case 0: new_game(); break;
                    case 1: load_game(); break;
                    case 2: show_scores(); break;
                    case 3: settings(); break;
                    case 4: profile(); break;
                    case 5: exit_program(); break;
                }
                break;
        }
    }

    endwin(); 
    return 0;
}

void new_game() {
    clear();
    refresh();
    main_map();
    getch();
}

void load_game() {
    clear();
    main_map();
    refresh();
    getch();
}

void show_scores() {
    clear();
    qsort(users, user_c, sizeof(User), compare);
    FILE *file = fopen("users_sorted.txt", "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    for (int i = 0; i < user_c; i++) {
        fprintf(file, "%s %s %s %d %d %d %d\n", users[i].username, users[i].password, users[i].email, users[i].score, users[i].gold, users[i].games, users[i].playtime);
    }
    fclose(file);
    mvprintw(5, 5, "Score board:");
    start_color();
    init_pair(1,COLOR_RED,COLOR_WHITE);
    init_pair(2,COLOR_CYAN,COLOR_WHITE);
    init_pair(3,COLOR_MAGENTA,COLOR_WHITE);
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(6,5,"GOAT:%d. %s -Score: %d -Gold: %d -Gamesplayed: %d -Playtime: %d", 1, users[0].username, users[0].score, users[0].gold, users[0].games, users[0].playtime);
    attroff(COLOR_PAIR(1) | A_BOLD);
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(7,5,"LEGEND:%d. %s -Score: %d -Gold: %d -Gamesplayed: %d -Playtime: %d", 2, users[1].username, users[1].score, users[1].gold, users[1].games, users[1].playtime);
    attroff(COLOR_PAIR(2) | A_BOLD);
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(8,5,"LEGEND:%d. %s -Score: %d -Gold: %d -Gamesplayed: %d -Playtime: %d", 3, users[2].username, users[2].score, users[2].gold, users[2].games, users[2].playtime);
    attroff(COLOR_PAIR(3) | A_BOLD);
    for (int i = 3; i < 10 && i < user_c; i++) {
        mvprintw(i + 6, 5, "%d. %s -Score: %d -Gold: %d -Gamesplayed: %d -Playtime: %d", i + 1, users[i].username, users[i].score, users[i].gold, users[i].games, users[i].playtime);
    }
    mvprintw(16, 5, "Press Enter to return to the menu.");
    refresh();
    getch();
}

void settings() {
    const char *settings[]={"Difficulty","Color"};
    draw_border();
    int choice=0;
    while(1){
        mvprintw(1,1,"Settings");
        for (int i = 0; i < 2; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", settings[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    switch(choice){
        case 0:
        clear();
    const char *levels[] = {"Easy", "Medium", "Hard"};
    choice = 0;
    while (1) {
        draw_border();
        mvprintw(1, 1, "Select Difficulty:");
        for (int i = 0; i < 3; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", levels[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    /*switch (choice)
    {
    case 0:
        
        break;
    case 1:
        g->max_items = MEDIUM_ITEMS;
        g->time_limit = MEDIUM_TIME;
        break;
    case 2:
        g->max_items = HARD_ITEMS;
        g->time_limit = HARD_TIME;
        break;
    }*/
    clear();
        break;
        case 1:
        clear();
        const char* colors[]={"WHITE","YELLOW","RED","BLUE"};
        choice=0;
        while (1) {
        draw_border();
        mvprintw(1, 1, "Select Color:");
            for (int i = 0; i < 4; ++i) {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", colors[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 3 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 3) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
        break;
    } 
}


void profile() {
    clear();
    mvprintw(5, 5, "Player Profile:");
    mvprintw(6, 5, "Name: Player A");
    mvprintw(7, 5, "Level: 1");
    mvprintw(8, 5, "Press Enter to return to the menu.");
    refresh();
    getch();
}

void exit_program() {
    clear();
    mvprintw(5, 5, "Goodbye!");
    refresh();
    getch();
    endwin();
    exit(0);
}

void draw_border() {
    clear();
    for (int x = 0; x < COLS; ++x) {
        mvprintw(0, x, "-");
        mvprintw(LINES - 1, x, "-");
    }
    for (int y = 0; y < LINES; ++y) {
        mvprintw(y, 0, "|");
        mvprintw(y, COLS - 1, "|");
    }
}

#endif game_menu_h