#ifndef login_menu_h
#define login_menu_h

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "user.h"
#include "game_menu.h"


User users[100]; 
int user_count = 0; 


void read_users_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%s %s", users[user_count].username, users[user_count].password) != EOF) {
        user_count++;
    }

    fclose(file);
}
void draw_menu_border()
{
    clear();
    for (int x = 0; x < COLS; ++x)
    {
        mvprintw(0, x, "-");
        mvprintw(LINES - 1, x, "-");
    }
    for (int y = 0; y < LINES; ++y)
    {
        mvprintw(y, 0, "|");
        mvprintw(y, COLS - 1, "|");
    }
}

void displayMenu(WINDOW *menu_win, int highlight) {
    char *choices[] = { "Login", "Guest", "Exit" };
    int n_choices = sizeof(choices) / sizeof(char *);

    box(menu_win, 0, 0);
    for (int i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, i + 1, 1, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, i + 1, 1, "%s", choices[i]);
        }
    }
    wrefresh(menu_win);
}


void login(WINDOW *menu_win) {
    char username[50];
    char password[50];

    mvwprintw(menu_win, 7, 1, "Username: ");
    wrefresh(menu_win);
    echo(); 
    wgetstr(menu_win, username);

    mvwprintw(menu_win, 8, 1, "Password: ");
    wrefresh(menu_win);
    wgetstr(menu_win, password);
    noecho(); 

    int authenticated = 0;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            authenticated = 1;
            break;
        }
    }

    if (authenticated) {
        mvwprintw(menu_win, 10, 1, "Login successful!");
        werase(menu_win);
        strcpy(player.username,username);
        strcpy(player.password,password);
        main_game_menu();
    } else {
        mvwprintw(menu_win, 10, 1, "Invalid username or password.");
    }
    wrefresh(menu_win);
    wgetch(menu_win);
    werase(menu_win); 
    box(menu_win, 0, 0); 
}

void guest(WINDOW *menu_win) {
    mvwprintw(menu_win, 10, 1, "Logged in as Guest.");
    wrefresh(menu_win);
    wgetch(menu_win);
    werase(menu_win);
    main_game_menu();
    box(menu_win, 0, 0); 
}

int main_login_menu() {
    read_users_from_file("users.txt"); 
    draw_menu_border();
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int highlight = 1;
    int choice = 0;
    int c;
    WINDOW *menu_win = newwin(12, 25, 1, 1); 
    keypad(menu_win, TRUE);

    while (1) {
        displayMenu(menu_win, highlight);
        c = wgetch(menu_win);
        draw_menu_border();
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = 3;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == 3)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: 
                choice = highlight;
                break;
            default:
                break;
        }

        if (choice == 1) {
            werase(menu_win); 
            box(menu_win, 0, 0); 
            draw_menu_border();
            wrefresh(menu_win); 
            login(menu_win);
            choice = 0;
        } else if (choice == 2) {
            werase(menu_win); 
            box(menu_win, 0, 0); 
            draw_menu_border();
            wrefresh(menu_win); 
            guest(menu_win);
            main_game_menu();
            choice = 0;
        } else if (choice == 3) {
            endwin();
            exit(0);
        }
    }

    endwin();
    return 0;
}

#endif login_menu_h