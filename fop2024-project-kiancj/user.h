#ifndef user_h
#define user_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
typedef struct {
    char username[50];
    char password[50];
    char email[100];
    int score;
    int gold;
    int playtime;
    int games;
    int health;
    char color[50];
} User;
User player;


#endif user_h