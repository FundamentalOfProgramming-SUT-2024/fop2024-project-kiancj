#ifndef sign_in_menu_h
#define sign_in_menu_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "user.h"
#include "map.h"
#include "game_menu.h"

User users[100];


int validate_username(char *name, int count) {
    for(int i = 0; i < count; i++) {
        if(strcmp(users[i].username, name) == 0) {
            return 0;
        }
    }
    return 1;
}


int validate_password_len(char *pass) {
    if(strlen(pass) >= 7) {
        return 1;
    } else {
        return 0;
    }
}


int validate_password_strength(char *pass) {
    int hasLower = 0, hasUpper = 0, hasDigit = 0;

    for(int i = 0; pass[i] != '\0'; i++) {
        if(pass[i] >= 'a' && pass[i] <= 'z') hasLower = 1;
        else if(pass[i] >= 'A' && pass[i] <= 'Z') hasUpper = 1;
        else if(pass[i] >= '0' && pass[i] <= '9') hasDigit = 1;
    }

    return (hasLower + hasUpper + hasDigit) >= 3;
}


int validate_email(char *email) {
    int atIndex = -1, dotIndex = -1;
    int length = strlen(email);

    for(int i = 0; i < length; i++) {
        if(email[i] == '@') {
            atIndex = i;
            break;
        }
    }
    if(atIndex == -1) return 0;

    for(int i = atIndex + 1; i < length; i++) {
        if(email[i] == '.') {
            dotIndex = i;
            break;
        }
    }
    if(dotIndex == -1 || atIndex == 0 || atIndex == length - 1 || dotIndex == atIndex + 1 || dotIndex == length - 1) return 0;

    return 1;
}
void border_draw()
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


void createUser(User *users, int *count, FILE *file) {
    char username[50], password[50], email[100];

    echo(); 
    mvprintw(5, 1, "Enter username: ");
    scanw("%s", username);
    if(validate_username(username, *count) == 0) {
        mvprintw(6, 1, "Username already used.");
        return;
    }
    
    mvprintw(7, 1, "Enter password: ");
    scanw("%s", password);
    if(validate_password_len(password) == 0) {
        mvprintw(8, 1, "Password is too short.");
        return;
    }
    if(validate_password_strength(password) == 0) {
        mvprintw(9, 1, "Password is not strong enough.");
        return;
    }

    mvprintw(10, 1, "Enter email: ");
    scanw("%s", email);
    if(validate_email(email) == 0) {
        mvprintw(11, 1, "Wrong Email format.");
        return;
    }

    strcpy(users[*count].username, username);
    strcpy(users[*count].password, password);
    strcpy(users[*count].email, email);
    users[*count].score=0;
    users[*count].gold=0;
    users[*count].playtime=0;
    users[*count].games=0;
    users[*count].health=100;
    strcpy(player.username, username);
    strcpy(player.password, password);
    strcpy(player.email, email);
    player.score=0;
    player.gold=0;
    player.playtime=0;
    player.games=0;
    player.health=100;
    fprintf(file, "%s %s %s %d %d %d %d\n", users[*count].username, users[*count].password, users[*count].email,users[*count].score,users[*count].gold,users[*count].games,users[*count].playtime);
    fflush(file);

    (*count)++;
    mvprintw(12, 1, "User created successfully!");

    noecho(); 
}


void displaymenu(User *users, int *userCount, FILE *file) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    char *choices[] = {
        "Create New User",
        "Exit",
    };
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight = 0;
    int choice = 0;
    int c;

    while(1) {
        clear();
        border_draw();
        for(int i = 0; i < n_choices; i++) {
            if(i == highlight) {
                attron(A_REVERSE);
                mvprintw(i + 1, 1, "%s", choices[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(i + 1, 1, "%s", choices[i]);
            }
        }
        refresh();

        c = getch();

        switch(c) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
                break;
            case 10: 
                choice = highlight;
                break;
            default:
                break;
        }

        if (c == 10) { 
            switch(choice) {
                case 0:
                    clear();
                    createUser(users, userCount, file);
                    getch(); 
                    break;
                case 1:
                    endwin();
                    printf("Exiting...\n");
                    exit(0);
            }
        }
    }
}


int loadUsers(User *users, FILE *file) {
    int count = 0;
    while(fscanf(file, "%s %s %s", users[count].username, users[count].password, users[count].email) != EOF) {
        count++;
    }
    return count;
}

int main_sign_in_menu() {
    int userCount = 0;

    FILE *file = fopen("users.txt", "a+");
    if(file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    userCount = loadUsers(users, file);

    displaymenu(users, &userCount,file);
    main_game_menu();
    fclose(file);
    return 0;
}

#endif sign_in_menu_h
