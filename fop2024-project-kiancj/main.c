#include <ncurses.h>
#include <stdlib.h>

#include "map.h"
#include "game_menu.h"
#include "login_menu.h"
#include "sign_in_menu.h"
#include "user.h"

void print_menu(WINDOW *menu_win, int highlight);

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();

    char *choices[] = {
        "Sign in",
        "Login",
    };
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight = 0;

    WINDOW *menu_win = newwin(10, 40, 4, 4);
    keypad(menu_win, TRUE);
    refresh();
    print_menu(menu_win, highlight);

    while (1)
    {
        int c = wgetch(menu_win);
        switch (c)
        {
        case KEY_UP:
            highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
            break;
        case 10: 
            switch (highlight)
            {
            case 0:
                main_sign_in_menu();
                break;
            case 1:
                main_login_menu();
                break;
            }
            break;
        default:
            break;
        }
        print_menu(menu_win, highlight);
    }

    endwin();
    return 0;
}

void print_menu(WINDOW *menu_win, int highlight)
{
    int x = 2, y = 2;
    box(menu_win, 0, 0);
    char *choices[] = {
        "Sign in",
        "Login",
    };
    int n_choices = sizeof(choices) / sizeof(char *);

    for (int i = 0; i < n_choices; ++i)
    {
        if (highlight == i)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
        {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }
    wrefresh(menu_win);
}
