#ifndef map_h
#define map_h


#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<math.h>
#include<assert.h>
#include<unistd.h>
#include <locale.h>
#include <stdbool.h>
#include <wchar.h>



///V stands for speed
////P stands for damage
////H stands for health

#define HARD_HEALTH 600
#define MEDIUM_HEALTH 800
#define EASY_HEALTH 1000
#define MAX_INVENTORY 10
#define COLOR_ORANGE 11
#define MAP_WIDTH  80
#define MAP_HEIGHT 24
#define ROOM_COUNT 6
#define MAX_ATTEMPTS 3
#define PASSCODE_LENGTH 4
#define MACE 0
#define DAGGER 1
#define MAGIC_WAND 2
#define NORMAL_ARROW 3
#define SWORD 4
#define HEALTH 0
#define POWER 1
#define SPEED 2



int global=0;

int min(int a, int b){
    if(a>b)
    return b;
    return a;
}
int max(int a, int b){
    if(a>b)
    return a;
    return b;
}
char** map;
int show[24][80];
typedef struct {
    char items[30];
    int count;
} Food;
typedef struct{
    int xpos;
    int ypos;
    int health;
    int damage;
}Monster;

typedef struct {
    char items[30];
    int count;
    int damage;
} Weapon;

typedef struct {
    char items[30];
    int count;
} Drug;


typedef struct {
    int xpos;
    int ypos;
    int health;
    int a_num;
    int level_floor;
    Food food[5];
    Weapon weapon[5];
    Drug drug[5];
    int hungriness;
    int gold;
    int curr_weapon;
} Player;

typedef struct {
    int x, y;
    int width, height;
    int door_x, door_y;
    int door_h_x,door_h_y;
    char passcode[PASSCODE_LENGTH + 1];
    int attempts_left;
    time_t lock_time; // Time when the door was locked
} Room;

typedef struct {
    int x;
    int y;
} Position;
Position parent[MAP_HEIGHT][MAP_WIDTH];
Position doors[ROOM_COUNT];

// Function to check if a key is in the inventory
/*bool has_key(Inventory *inv, char key) {
    for (int i = 0; i < inv->count; i++) {
        if (inv->items[i] == key) {
            return true;
        }
    }
    return false;
}

// Function to remove a key from the inventory
void remove_key(Inventory *inv, char key) {
    for (int i = 0; i < inv->count; i++) {
        if (inv->items[i] == key) {
            for (int j = i; j < inv->count - 1; j++) {
                inv->items[j] = inv->items[j + 1];
            }
            inv->count--;
            break;
        }
    }
}

// Function to use a master key to unlock a door
bool use_master_key(Inventory *inv, bool door_locked) {
    if (!has_key(inv, 'A')) {
        printw("You don't have a master key.\n");
        return door_locked;
    }

    if ((rand() % 10) < 1) { // 10% chance key breaks
        printw("The master key broke while trying to unlock the door!\n");
        remove_key(inv, 'A');
        return door_locked;
    }

    printw("You used the master key to unlock the door!\n");
    remove_key(inv, 'A');
    return false; // Door is unlocked
}*/
int check_monster(Monster * monster){
    if(monster->health<=0){
        return 1;
    }
    return 0;
}
void generate_passcode(char* passcode) {
    for (int i = 0; i < PASSCODE_LENGTH; ++i) {
        passcode[i] = '0' + rand() % 10;
    }
    passcode[PASSCODE_LENGTH] = '\0';
}
void handle_input(char c, Player* player, Room rooms[]);
void draw_room(Room room, int index,int level);
void generate_rooms(Room rooms[], int count,int level);
void check_position(int new_y, int new_x, Player* player, Room rooms[]);
char active_passcode[PASSCODE_LENGTH + 1] = "";
time_t passcode_start_time;

void display_passcode(const char *passcode) {
    mvprintw(0, 12, "Passcode: %s", passcode);
}

void clear_passcode_display() {
    mvprintw(0, 0, "Passcode:                ");
}
int mark[24][80];
int check_map(int x, int y){
    if(map[x][y]=='.')
    return 1;
    return 0;
}


int check(int x, int y, int resx, int resy) {
    if(x == resx && y == resy)
        return 1;
    if(x >= 0 && x <24 && y >= 0 && y <80 && mark[x][y] == 0) {
        if(map[x][y] == '_')
            return 0;
        if(map[x][y] == '|')
            return 0;
        if(map[x][y] == '.')
            return 0;
        if(map[x][y] == 'P')
            return 1;
        else if(map[x][y] == 0)
            return 1;
        else if(map[x][y]=='#')
            return 1;
    }
    return 0;
}

int dfs(int bx, int by, int x, int y, int resx, int resy) {
    mark[x][y] = 1;
    int dx[4] = {0, 0, 0, 0};
    int dy[4] = {0, 0, 0, 0};
    if(x == resx && y ==  resy) {
        return 1;
    }
    if(x == resx && y < resy) {
        dy[0] = 1;
        dx[1] = 1;
        dx[2] = -1;
        dy[3] = -1;
    }
    else if(x == resx && y > resy) {
        dy[0] = -1;
        dx[1] = -1;
        dx[2] = 1;
        dy[3] = 1;
    }
    else if(x > resx && y == resy) {
        dx[0] = -1;
        dy[1] = 1;
        dy[2] = -1;
        dx[3] = 1;
    }
    else if(x < resx && y == resy) {
        dx[0] = 1;
        dy[1] = -1;
        dy[2] = 1;
        dx[3] = -1;
    }
    if(x < resx && y < resy) {
        dx[0] = 1;
        dy[1] = 1;
        dx[2] = -1;
        dy[3] = -1;
    }
    else if(x < resx && y > resy) {
        dx[0] = 1;
        dy[1] = -1;
        dx[2] = -1;
        dy[3] = 1;
    }
    else if(x > resx && y < resy) {
        dx[0] = -1;
        dy[1] = 1;
        dx[2] = 1;
        dy[3] = -1;
    }
    else if(x > resx && y > resy) {
        dx[0] = -1;
        dy[1] = -1;
        dx[2] = 1;
        dy[3] = 1;
    }
    for(int i = 0; i < 4; i++) {
        int b=check(x + dx[i], y + dy[i], resx, resy);
        if(b) {
            if(dfs(bx, by, x + dx[i], y + dy[i], resx, resy)) {
                    if(!(bx==x && by==y)){
                        map[x][y] = '#';
                        attron(COLOR_PAIR(6));
                        mvprintw(x,y,"#");
                        attroff(COLOR_PAIR(6));
                        return 1;
                    }
            }
        }
    }
    return 0;
}
 int  reduce_health(Player * player,Monster * monster){
    int d=monster->damage;
    if(player->xpos==monster->xpos){
        if(player->ypos==monster->ypos+1){
            player->health-=d;
            return 1;
        
        }
        else if(player->ypos==monster->ypos-1){
            player->health-=d;
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(player->xpos==monster->xpos-1){
        if(player->ypos==monster->ypos+1){
            player->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos-1){
            player->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos){
            player->health-=d;
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(player->xpos==monster->xpos+1){
        if(player->ypos==monster->ypos+1){
            player->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos-1){
            player->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos){
            player->health-=d;
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}
void draw_room(Room room, int index,int level) {
    
    if(index==1){
        for (int i = 0; i < room.height; ++i) {
        for (int j = 0; j < room.width; ++j) {
            if (i == 0 || i == room.height - 1) {
                 attron(COLOR_PAIR(2));
                mvprintw(room.y + i, room.x + j, "-");
                attroff(COLOR_PAIR(2));
            } else if (j == 0 || j == room.width - 1) {
                 attron(COLOR_PAIR(2));
                mvprintw(room.y + i, room.x + j, "|");
                attroff(COLOR_PAIR(2));
            } else {
                if(map[room.y + i][room.x + j]==0){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, ".");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='f'){
                    attron(COLOR_PAIR(6));
                    mvprintw(room.y + i,room.x + j, "f");
                    attroff(COLOR_PAIR(6));
                }
                else if(map[room.y + i][room.x + j]=='F'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "F");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='U'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "U");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='S'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "S");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='D'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "D");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='G'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "G");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='s'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "s");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='d'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "d");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='n'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "n");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='V'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "V");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='H'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "H");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='P'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "P");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='w'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "w");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='.'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, ".");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='O'){
                    attron(COLOR_PAIR(4));
                    mvprintw(room.y + i,room.x + j, "O");
                    attroff(COLOR_PAIR(4));
                }
                else if(map[room.y + i][room.x + j]=='$'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i,room.x + j, "$");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='B'){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i,room.x + j,"BG");
                    attroff(COLOR_PAIR(10));
                }
                else if(map[room.y + i][room.x + j]=='^'){
                    attron(COLOR_PAIR(3));
                    mvprintw(room.y + i, room.x + j, "^");
                    attroff(COLOR_PAIR(3));
                }
                else if(map[room.y + i][room.x + j]=='&'){
                    attron(COLOR_PAIR(1));
                mvprintw(room.y + i, room.x + j, "&");
                attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='A'){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i, room.x + j,"A");
                    attroff(COLOR_PAIR(10));
                }
                
            }
        }
    }
    }
    else if(level==4 && index==5){
     
        for (int i = 0; i < room.height; ++i) {
        for (int j = 0; j < room.width; ++j) {
            if (i == 0 || i == room.height - 1) {
                 attron(COLOR_PAIR(10));
                mvprintw(room.y + i, room.x + j, "-");
                attroff(COLOR_PAIR(10));
            } else if (j == 0 || j == room.width - 1) {
                 attron(COLOR_PAIR(10));
                mvprintw(room.y + i, room.x + j, "|");
                attroff(COLOR_PAIR(10));
            } else {
                if(map[room.y + i][room.x + j]==0){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i, room.x + j, ".");
                    attroff(COLOR_PAIR(10));
                }
                else if(map[room.y + i][room.x + j]=='.'){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i, room.x + j, ".");
                    attroff(COLOR_PAIR(10));
                }
                else if(map[room.y + i][room.x + j]=='$'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i,room.x + j, "$");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='B'){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i,room.x + j,"B");
                    attroff(COLOR_PAIR(10));
                }
                else if(map[room.y + i][room.x + j]=='V'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "V");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='H'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "H");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='P'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "P");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='s'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "s");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='d'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "d");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='n'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "n");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='w'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "w");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='f'){
                    attron(COLOR_PAIR(6));
                    mvprintw(room.y + i,room.x + j, "f");
                    attroff(COLOR_PAIR(6));
                }
                else if(map[room.y + i][room.x + j]=='F'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "F");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='U'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "U");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='S'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "S");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='D'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "D");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='G'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "G");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='<'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "<");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='O'){
                    attron(COLOR_PAIR(4));
                    mvprintw(room.y + i,room.x + j, "O");
                    attroff(COLOR_PAIR(4));
                }
                else if(map[room.y + i][room.x + j]=='^'){
                    attron(COLOR_PAIR(3));
                    mvprintw(room.y + i, room.x + j, "^");
                    attroff(COLOR_PAIR(3));
                }
                else if(map[room.y + i][room.x + j]=='&'){
                    attron(COLOR_PAIR(1));
                mvprintw(room.y + i, room.x + j, "&");
                attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='A'){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i, room.x + j,"A");
                    attroff(COLOR_PAIR(10));
                }
                
            }
        }
    }
    
}
    else{
    for (int i = 0; i < room.height; ++i) {
        for (int j = 0; j < room.width; ++j) {
            if (i == 0 || i == room.height - 1) {
                 attron(COLOR_PAIR(5));
                mvprintw(room.y + i, room.x + j, "-");
                attroff(COLOR_PAIR(5));
            } else if (j == 0 || j == room.width - 1) {
                 attron(COLOR_PAIR(5));
                mvprintw(room.y + i, room.x + j, "|");
                attroff(COLOR_PAIR(5));
            } else {
                if(map[room.y + i][room.x + j]==0){
                    attron(COLOR_PAIR(5));
                    mvprintw(room.y + i, room.x + j, ".");
                    attroff(COLOR_PAIR(5));
                }
                else if(map[room.y + i][room.x + j]=='.'){
                    attron(COLOR_PAIR(5));
                    mvprintw(room.y + i, room.x + j, ".");
                    attroff(COLOR_PAIR(5));
                }
                else if(map[room.y + i][room.x + j]=='f'){
                    attron(COLOR_PAIR(6));
                    mvprintw(room.y + i,room.x + j, "f");
                    attroff(COLOR_PAIR(6));
                }
                else if(map[room.y + i][room.x + j]=='F'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "F");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='U'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "U");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='S'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "S");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='D'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "D");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='G'){
                    attron(COLOR_PAIR(1));
                    mvprintw(room.y + i,room.x + j, "G");
                    attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='s'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "s");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='d'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "d");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='n'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "n");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='w'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i, room.x + j, "w");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='V'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "V");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='H'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "H");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='P'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "P");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='$'){
                    attron(COLOR_PAIR(8));
                    mvprintw(room.y + i,room.x + j, "$");
                    attroff(COLOR_PAIR(8));
                }
                else if(map[room.y + i][room.x + j]=='B'){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i,room.x + j,"B");
                    attroff(COLOR_PAIR(10));
                }
                else if(map[room.y + i][room.x + j]=='<'){
                    attron(COLOR_PAIR(2));
                    mvprintw(room.y + i, room.x + j, "<");
                    attroff(COLOR_PAIR(2));
                }
                else if(map[room.y + i][room.x + j]=='O'){
                    attron(COLOR_PAIR(4));
                    mvprintw(room.y + i,room.x + j, "O");
                    attroff(COLOR_PAIR(4));
                }
                else if(map[room.y + i][room.x + j]=='^'){
                    attron(COLOR_PAIR(3));
                    mvprintw(room.y + i, room.x + j, "^");
                    attroff(COLOR_PAIR(3));
                }
                else if(map[room.y + i][room.x + j]=='&'){
                    attron(COLOR_PAIR(1));
                mvprintw(room.y + i, room.x + j, "&");
                attroff(COLOR_PAIR(1));
                }
                else if(map[room.y + i][room.x + j]=='A'){
                    attron(COLOR_PAIR(10));
                    mvprintw(room.y + i, room.x + j,"A");
                    attroff(COLOR_PAIR(10));
                }
                
            }
        }
    }
    }
    for(int i=0;i<24;i++){
        for(int j=0;j<80;j++){
            if(map[i][j]=='#'){
                attron(COLOR_PAIR(6));
                mvprintw(i,j,"#");
                attroff(COLOR_PAIR(6));
            }
        }
    }
    doors[index].x = room.door_x;
    doors[index].y = room.door_y;
    if(index==1){
        attron(COLOR_PAIR(3));
        mvprintw(room.door_h_y, room.door_h_x, "?");
        map[room.door_h_y][room.door_h_x]='?';
        attroff(COLOR_PAIR(3));
    }
    if(index!=0){
        attron(COLOR_PAIR(6));
        mvprintw(room.door_y, room.door_x, "+");
        attroff(COLOR_PAIR(6));
        map[room.door_y][room.door_x] = '+';
    }
    else if(map[room.door_y][room.door_x]=='@'){
        attron(COLOR_PAIR(2));
        mvprintw(room.door_y, room.door_x, "@");
        attroff(COLOR_PAIR(2));
    }
    else {
        attron(COLOR_PAIR(6));
        mvprintw(room.door_y, room.door_x, "+");
        attroff(COLOR_PAIR(6));
        map[room.door_y][room.door_x] = '+';
    }
     
    
}
int damage_enemies_melee(Player * player,Monster * monster,int weapon){
    int d;
    if(weapon==MACE){
        d=5;
    }
    else if(weapon==SWORD){
        d=10;
    }
    if(player->xpos==monster->xpos){
        if(player->ypos==monster->ypos+1){
            monster->health-=d;
            return 1;
        
        }
        else if(player->ypos==monster->ypos-1){
            monster->health-=d;
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(player->xpos==monster->xpos-1){
        if(player->ypos==monster->ypos+1){
            monster->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos-1){
            monster->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos){
            monster->health-=d;
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(player->xpos==monster->xpos+1){
        if(player->ypos==monster->ypos+1){
            monster->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos-1){
            monster->health-=d;
            return 1;
        }
        else if(player->ypos==monster->ypos){
            monster->health-=d;
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}


void generate_rooms(Room rooms[], int count,int level) {
    
    for (int i = 0; i < count; ++i) {   
            rooms[i].width = 6 + rand() % 4;
            rooms[i].height = 6 + rand() % 4;
            if(i<3){
                rooms[i].x = rand() % 3 + (MAP_WIDTH / 3) * i + 5;
                rooms[i].y = 3;
            }
            else{
                rooms[i].x = rand() % 3 + (MAP_WIDTH / 3) * (i%3) + 5;
                rooms[i].y = rand() % 4 +(MAP_HEIGHT/2);
            }

        int door_position = rand() % 4;
        if(i==1){
            rooms[i].door_x = rooms[i].x;
            rooms[i].door_y = rooms[i].y + rooms[i].height / 2;
            rooms[i].door_h_x = rooms[i].x + rooms[i].width - 1;
            rooms[i].door_h_y = rooms[i].y + rooms[i].height / 2;
            map[rooms[i].door_h_y][rooms[i].door_h_x]='?';
        }
        else if(i==5){
            rooms[i].door_x = rooms[i].x;
            rooms[i].door_y = rooms[i].y + rooms[i].height / 2;
        }
        else{
        if (door_position == 0) {
            rooms[i].door_x = rooms[i].x + rooms[i].width / 2;
            rooms[i].door_y = rooms[i].y;
        } else if (door_position == 1) {
            rooms[i].door_x = rooms[i].x + rooms[i].width / 2;
            rooms[i].door_y = rooms[i].y + rooms[i].height - 1;
        } else if (door_position == 2) {
            rooms[i].door_x = rooms[i].x;
            rooms[i].door_y = rooms[i].y + rooms[i].height / 2;
        } else {
            rooms[i].door_x = rooms[i].x + rooms[i].width - 1;
            rooms[i].door_y = rooms[i].y + rooms[i].height / 2;
        }
        }
        for(int f=0;f<2;f++){
            //food
        int Y_F = (rand() % (rooms[i].height - 4)) + rooms[i].y + 2;
        int X_F = (rand() % (rooms[i].width - 4)) + rooms[i].x + 2;
        map[Y_F][X_F] = 'f';
        ///////////////////////
        }
        
        //O
    int Y_O = (rand() % (rooms[i].height - 4)) + rooms[i].y + 2;
    int X_O = (rand() % (rooms[i].width - 4)) + rooms[i].x + 2;
    map[Y_O][X_O] = 'O';
    ///////////////////////
    
        for (int t = 0; t < rooms[i].height; ++t) {
        for (int j = 0; j < rooms[i].width; ++j) {
            if (t == 0 || t == rooms[i].height - 1) {
                map[rooms[i].y + t][rooms[i].x + j] = '-';
            } else if (j == 0 || j == rooms[i].width - 1) {
                map[rooms[i].y + t][rooms[i].x + j] = '|';
            } else {
                if(map[rooms[i].y + t][rooms[i].x + j]==0){
                    map[rooms[i].y + t][rooms[i].x + j] = '.';
                }
                
            }
        }
    }

    
    //trap
    // Place the trap
    if(level!=4){
        int trap_placed = 0;
    while (!trap_placed) {
        int Y_T = (rand() % (rooms[i].height - 2)) + rooms[i].y + 1;
        int X_T = (rand() % (rooms[i].width - 2)) + rooms[i].x + 1;

        // Ensure the trap is not placed on the door or other objects
        if (map[Y_T][X_T] == '.') {
            map[Y_T][X_T] = '^';
            trap_placed = 1;
        }
    }
    }
    else{
        for(int f=0;f<3;f++){
            int trap_placed = 0;
    while (!trap_placed) {
        int Y_T = (rand() % (rooms[i].height - 2)) + rooms[i].y + 1;
        int X_T = (rand() % (rooms[i].width - 2)) + rooms[i].x + 1;

        // Ensure the trap is not placed on the door or other objects
        if (map[Y_T][X_T] == '.') {
            map[Y_T][X_T] = '^';
            trap_placed = 1;
        }
    }
        }
    }
            //normal gold
    int gold_placed=0;
    while (!gold_placed) {
        int Y_G = (rand() % (rooms[i].height - 2)) + rooms[i].y + 1;
        int X_G = (rand() % (rooms[i].width - 2)) + rooms[i].x + 1;

        // Ensure the pass_generator is not placed on the door or other objects
            if(map[Y_G][X_G]=='.'){
                map[Y_G][X_G] = '$';
                gold_placed = 1;
            }
            
    }



    }
        //dagger
    int dagger_placed=0;
    while (!dagger_placed) {
        int Y_D = (rand() % (rooms[1].height - 2)) + rooms[1].y + 1;
        int X_D = (rand() % (rooms[1].width - 2)) + rooms[1].x + 1;

        // Ensure the dagger is not placed on the door or other objects
            if(map[Y_D][X_D]=='.'){
                map[Y_D][X_D] = 'd';
                dagger_placed = 1;
            }
            
    }
    ///speed drug
    int speed_placed=0;
    while (!speed_placed) {
        int Y_V = (rand() % (rooms[2].height - 2)) + rooms[1].y + 1;
        int X_V = (rand() % (rooms[2].width - 2)) + rooms[1].x + 1;

        // Ensure the dagger is not placed on the door or other objects
            if(map[Y_V][X_V]=='.'){
                map[Y_V][X_V] = 'V';
                speed_placed = 1;
            }
            
    }
    ///health drug
    int health_placed=0;
    while (!health_placed) {
        int Y_H = (rand() % (rooms[3].height - 2)) + rooms[2].y + 1;
        int X_H = (rand() % (rooms[3].width - 2)) + rooms[2].x + 1;

        // Ensure the dagger is not placed on the door or other objects
            if(map[Y_H][X_H]=='.'){
                map[Y_H][X_H] = 'H';
                health_placed = 1;
            }
            
    }

        //damage drug
    int damage_placed=0;
    while (!damage_placed) {
        int Y_D_D = (rand() % (rooms[4].height - 2)) + rooms[2].y + 1;
        int X_D_D = (rand() % (rooms[4].width - 2)) + rooms[2].x + 1;

        // Ensure the dagger is not placed on the door or other objects
            if(map[Y_D_D][X_D_D]=='.'){
                map[Y_D_D][X_D_D] = 'P';
                damage_placed = 1;
            }
            
    }
    //magic wound
    int wound_placed=0;
    while (!wound_placed) {
        int Y_W = (rand() % (rooms[3].height - 2)) + rooms[3].y + 1;
        int X_W = (rand() % (rooms[3].width - 2)) + rooms[3].x + 1;

        // Ensure the dagger is not placed on the door or other objects
            if(map[Y_W][X_W]=='.'){
                map[Y_W][X_W] = 'w';
                wound_placed = 1;
            }
            
    }
    //normal arrow
    int n_arrow_placed=0;
    while (!n_arrow_placed) {
        int Y_A = (rand() % (rooms[4].height - 2)) + rooms[4].y + 1;
        int X_A = (rand() % (rooms[4].width - 2)) + rooms[4].x + 1;

        // Ensure the dagger is not placed on the door or other objects
            if(map[Y_A][X_A]=='.'){
                map[Y_A][X_A] = 'n';
                n_arrow_placed = 1;
            }
            
    }
    //pass_generator
    int pass_placed=0;
    while (!pass_placed) {
        int Y_P_G = (rand() % (rooms[0].height - 2)) + rooms[0].y + 1;
        int X_P_G = (rand() % (rooms[0].width - 2)) + rooms[0].x + 1;

        // Ensure the pass_generator is not placed on the door or other objects
        if (map[Y_P_G][X_P_G] == '.') {
            map[Y_P_G][X_P_G] = '&';
            pass_placed = 1;
        }
    }
    //stairs
    int stairs_palced=0;
    while (!stairs_palced) {
        int Y_S = (rand() % (rooms[5].height - 2)) + rooms[5].y + 1;
        int X_S= (rand() % (rooms[5].width - 2)) + rooms[5].x + 1;

        // Ensure the stairs is not placed on the door or other objects
        if (map[Y_S][X_S] == '.') {
            map[Y_S][X_S] = '<';
            stairs_palced = 1;
        }
    }
       //ancient key
    int ancient_pres=0;
    while (!ancient_pres) {
        int Y_A = (rand() % (rooms[4].height - 2)) + rooms[4].y + 1;
        int X_A = (rand() % (rooms[4].width - 2)) + rooms[4].x + 1;

        // Ensure the pass_generator is not placed on the door or other objects
           if(map[Y_A][X_A]=='.'){
            map[Y_A][X_A] = 'A';
            ancient_pres = 1;
           }
           
    }

    //black gold
    int black_placed=0;
    while (!black_placed) {
        int Y_B_G = (rand() % (rooms[4].height - 2)) + rooms[4].y + 1;
        int X_B_G = (rand() % (rooms[4].width - 2)) + rooms[4].x + 1;

        // Ensure the pass_generator is not placed on the door or other objects
           if(map[Y_B_G][X_B_G]=='.'){
            map[Y_B_G][X_B_G] = 'B';
            black_placed = 1;
           }
    }
    // Generate passcode
    generate_passcode(rooms[0].passcode);
    rooms[0].attempts_left = MAX_ATTEMPTS;
    rooms[0].lock_time = 0; // Initially unlocked
    
    
    map[rooms[0].door_y][rooms[0].door_x]='@';
    
}

Player* setup_player(Room rooms[], int room_count) {
    Player* player = malloc(sizeof(Player));
   

    Room room = rooms[0];

    player->xpos = room.x + 1 + rand() % (room.width - 2);
    player->ypos = room.y + 1 + rand() % (room.height - 2);


    mvprintw(player->ypos, player->xpos, "P");
    move(player->ypos, player->xpos);
    return player;
}

Monster* setup_monster(Room rooms[], int room_count,char c,int i) {
    Monster* monster = malloc(sizeof(Monster));
    Room room = rooms[i];
    int monster_placed=0;
    while(!monster_placed){
        monster->xpos = room.x + 1 + rand() % (room.width - 2);
        monster->ypos = room.y + 1 + rand() % (room.height - 2);
        if(map[monster->ypos][monster->xpos]=='.'){
            monster_placed=1;
        }
    }
   
    map[monster->ypos][monster->xpos]=c;
    attron(COLOR_PAIR(1));
    mvprintw(monster->ypos,monster->xpos,"%c",c);
    attroff(COLOR_PAIR(1));
    return monster;
}
void player_move(int y, int x, Player* player) {
    // Restore room color at previous position
    char prev_cell = map[player->ypos][player->xpos];
    switch (prev_cell) {
        case '<':
            attron(COLOR_PAIR(2)); 
            mvprintw(player->ypos, player->xpos, "<");
            attroff(COLOR_PAIR(2));
            break;
        case 'V':
            attron(COLOR_PAIR(2)); 
            mvprintw(player->ypos, player->xpos, "V");
            attroff(COLOR_PAIR(2));
            break;
        case 'P':
            attron(COLOR_PAIR(2)); 
            mvprintw(player->ypos, player->xpos, "P");
            attroff(COLOR_PAIR(2));
            break;
        case 'H':
            attron(COLOR_PAIR(2)); 
            mvprintw(player->ypos, player->xpos, "H");
            attroff(COLOR_PAIR(2));
            break;
        case 'B':
            map[player->ypos][player->xpos]='.';
            attron(COLOR_PAIR(5)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, ".");
            attroff(COLOR_PAIR(5));
            break;
        case '$':
            map[player->ypos][player->xpos]='.';
            attron(COLOR_PAIR(5)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, ".");
            attroff(COLOR_PAIR(5));
            break;
            case 's':
            attron(COLOR_PAIR(8)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, "s");
            attroff(COLOR_PAIR(8));
            break;
            case 'n':
            attron(COLOR_PAIR(8)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, "n");
            attroff(COLOR_PAIR(8));
            break;
            case 'd':
            attron(COLOR_PAIR(8)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, "d");
            attroff(COLOR_PAIR(8));
            break;

        case 'w':
            attron(COLOR_PAIR(8)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, "w");
            attroff(COLOR_PAIR(8));
            break;    
        case '?':
            attron(COLOR_PAIR(3));
            mvprintw(player->ypos, player->xpos, "?");
            attroff(COLOR_PAIR(3));
            break;
        case '#':
            attron(COLOR_PAIR(6)); 
            mvprintw(player->ypos, player->xpos, "#");
            attroff(COLOR_PAIR(6));
            break;
        case '.':
            attron(COLOR_PAIR(5)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, ".");
            attroff(COLOR_PAIR(5));
            break;
        case '+':
            attron(COLOR_PAIR(6));
            mvprintw(player->ypos, player->xpos, "+");
            attroff(COLOR_PAIR(6));
            break;
        case '^':
        attron(COLOR_PAIR(3));
        mvprintw(player->ypos, player->xpos, "^");
        attroff(COLOR_PAIR(3));
            break;
        case '&':
            attron(COLOR_PAIR(1));
            mvprintw(player->ypos, player->xpos, "&");
            attroff(COLOR_PAIR(1));
            break;
        case '@':
            attron(COLOR_PAIR(1));
            mvprintw(player->ypos, player->xpos, "@");
            attroff(COLOR_PAIR(1));
            break;
        case 'A':
            map[player->ypos][player->xpos]='.';
            attron(COLOR_PAIR(5)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, ".");
            attroff(COLOR_PAIR(5));
            break;
        case 'f':
            map[player->ypos][player->xpos]='.';
            attron(COLOR_PAIR(5)); // Assuming room color; adapt as needed
            mvprintw(player->ypos, player->xpos, ".");
            attroff(COLOR_PAIR(5));
            break;

    }

    // Move player
    player->ypos = y;
    player->xpos = x;

    // Draw player in white
    attron(COLOR_PAIR(7));
    mvprintw(player->ypos, player->xpos, "P");
    attroff(COLOR_PAIR(7));

    move(player->ypos, player->xpos);
}

void check_position(int new_y, int new_x, Player* player,Room rooms[]) {
    char pass[20];
    char c1;
    switch(map[new_y][new_x]) {
        case 'V':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->drug[SPEED].count++;
                map[new_y][new_x]='.';
                player_move(new_y, new_x, player);
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case 'P':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->drug[POWER].count++;
                map[new_y][new_x]='.';
                player_move(new_y, new_x, player);
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case 'H':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->drug[HEALTH].count++;
                map[new_y][new_x]='.';
                player_move(new_y, new_x, player);
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case 's':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->weapon[SWORD].count++;
                map[new_y][new_x]='.';
                player_move(new_y, new_x, player);
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case 'd':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->weapon[DAGGER].count++;
                map[new_y][new_x]='.';
                player_move(new_y, new_x, player);
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case 'n':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->weapon[NORMAL_ARROW].count++;
                map[new_y][new_x]='.';
                player_move(new_y, new_x, player);
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case 'w':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->weapon[MAGIC_WAND].count++;
                map[new_y][new_x]='.';
                player_move(new_y, new_x, player);
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case '$':
            player->gold+=10;
            player_move(new_y, new_x, player);
            mvprintw(24,10,"10 gold gained");
            refresh();
            move(player->ypos,player->xpos);
            break;
        case 'B':
            player->gold+=50;
            player_move(new_y, new_x, player);
            mvprintw(24,10,"50 gold gained");
            move(player->ypos,player->xpos);
            break;
        case '<':
             c1 = getchar();
            if((c1 == 'p') || (c1 == 'P')) {
                player->level_floor++;
            }
            else{
                player_move(new_y, new_x, player);
            }
            break;
        case '?':
            player_move(new_y, new_x, player);
            break;
        case '#':
            player_move(new_y, new_x, player);
            break;
        case '.':
            player_move(new_y, new_x, player);
            break;
        case '+':
            player_move(new_y, new_x, player);
            break;
        case '^':
            player->health -= 10; // Trap interaction
            player_move(new_y, new_x, player);
            break;
        case '&':
            generate_passcode(active_passcode);
            display_passcode(active_passcode);
            strcpy(rooms[0].passcode,active_passcode);
            passcode_start_time = time(NULL);
            player_move(new_y, new_x, player);
            break;
        case '@':
            
            if(player->a_num>0){
                // Attempt to use a master key
                    player_move(new_y, new_x, player);
                    map[new_y][new_x]='+';
                    refresh();
                
            }
                else{
                clear();
                refresh();
                echo();
                curs_set(TRUE);
                mvprintw(0, 0, "Enter the password:");
                scanw("%s",pass);
            if(strcmp(pass,rooms[0].passcode)==0){
                clear();
                for (int i = 0; i < ROOM_COUNT; i++) {
                        draw_room(rooms[i], i,player->level_floor);
                    }
                player_move(new_y, new_x, player);
                map[new_y][new_x]='+';
                refresh();
            }
            else {
                clear();
                rooms[0].attempts_left--;
                for (int i = 0; i < ROOM_COUNT; i++) {
                    draw_room(rooms[i], i,player->level_floor);
                }
                if(rooms[0].attempts_left==2){
                    attron(COLOR_PAIR(4));
                    mvprintw(rooms[0].door_y,rooms[0].door_x,"@");
                    attroff(COLOR_PAIR(4));
                }
                else if(rooms[0].attempts_left==1){
                    attron(COLOR_PAIR(8));
                    mvprintw(rooms[0].door_y,rooms[0].door_x,"@");
                    attroff(COLOR_PAIR(8));
                }
                else{
                    attron(COLOR_PAIR(1));
                    mvprintw(rooms[0].door_y,rooms[0].door_x,"@");
                    attroff(COLOR_PAIR(1));
                }
                move(player->ypos, player->xpos);
                noecho();
                curs_set(FALSE);
                refresh();
            }
                }
            
            break;
            case 'A':
            player->a_num++;
            player_move(new_y, new_x, player);
            break;
            case 'f':
            strcmp(player->food[0].items,"Simple food");
            if(player->food[0].count<5){
                player->food[0].count++;
                player_move(new_y, new_x, player);
                break;
            }
            else{
                move(player->ypos, player->xpos);
                break;
            }
            
         default:
             move(player->ypos, player->xpos);
             break;
    }
}
void draw_health_screen(Player *player, Room rooms[]) {
    clear(); // Clear the screen
    mvprintw(2, 5, "=== Player Status ===");
    
    // Draw health bar
    mvprintw(4, 3, "Hungriness: ");
    int bar_width = 20; // Max width of health bar
    int health_fill = (player->hungriness * bar_width) / 300; // Scale health

    // Draw filled part of the health bar
    attron(COLOR_PAIR(1));
    for (int i = 0; i < health_fill; i++) {
        mvaddch(4, 13 + i, '=');
    }
    attroff(COLOR_PAIR(1));

    // Draw empty part
    attron(COLOR_PAIR(2));
    for (int i = health_fill; i < bar_width; i++) {
        mvaddch(4, 13 + i, '-');
    }
    attroff(COLOR_PAIR(2));

    // Display food count
    mvprintw(6, 5, "Simple Food: %d", player->food[0].count);
    mvprintw(9, 5, "Precious Food: %d", player->food[1].count);
    mvprintw(12, 5, "Magic Food: %d", player->food[2].count);
    mvprintw(15, 5, "Poisoned Food: %d", player->food[3].count);
    mvprintw(60, 5, "Press 'F' to go back");
    refresh();
    char ch;
    ch=getch(); 
    switch(ch){
        
        case '1':
        player->food[0].count--;
        if(player->hungriness>15){
            player->hungriness-=15;
            if(player->health>985){
                player->health=1000;
            }
            else{
                 player->health+=15;
            }
           
        }
        else{
            player->hungriness=0;
            if(player->health>985){
                player->health=1000;
            }
            else{
                player->health+=15;
            }
            
        }
        clear();
        for (int i = 0; i < ROOM_COUNT; i++) {
                    draw_room(rooms[i], i,player->level_floor);
                }
        break;
        default:clear();
        for (int i = 0; i < ROOM_COUNT; i++) {
                    draw_room(rooms[i], i,player->level_floor);
                }
                break;
    }
   
    refresh();
}
void draw_weapon_screen(Player *player, Room rooms[]) {
    clear(); // Clear the screen
    mvprintw(2, 5, "=== WEAPONS ===");
    
    // Draw health bar

    mvprintw(4, 5, "CLOSE RANGE: ");

    mvprintw(10,5,"LONG RANGE: ");
    // Display weapons count
    if(player->curr_weapon==MACE){
        attron(COLOR_PAIR(2));
        mvprintw(6,2,"|>");
        attroff(COLOR_PAIR(2));
    }
    else if(player->curr_weapon==DAGGER){
        attron(COLOR_PAIR(2));
        mvprintw(13,2,"|>");
        attroff(COLOR_PAIR(2));
    }
    else if(player->curr_weapon==SWORD){
        attron(COLOR_PAIR(2));
        mvprintw(9,2,"|>");
        attroff(COLOR_PAIR(2));
    }
    else if(player->curr_weapon==NORMAL_ARROW){
        attron(COLOR_PAIR(2));
        mvprintw(16,2,"|>");
        attroff(COLOR_PAIR(2));
    }
    else if(player->curr_weapon==MAGIC_WAND){
        attron(COLOR_PAIR(2));
        mvprintw(19,2,"|>");
        attroff(COLOR_PAIR(2));
    }
    attron(COLOR_PAIR(8));
    mvprintw(6, 5, "MACE: %d", player->weapon[0].count);
    mvprintw(9, 5, "SWORD: %d", player->weapon[SWORD].count);
    mvprintw(13, 5, "DAGGER: %d", player->weapon[DAGGER].count);
    mvprintw(16, 5, "NORMAL_ARROW: %d", player->weapon[NORMAL_ARROW].count);
    mvprintw(19, 5, "MAGIC_WAND: %d", player->weapon[MAGIC_WAND].count);
    attroff(COLOR_PAIR(8));
    refresh();
    char ch;
    ch=getch(); 
    switch(ch){
        default:clear();
        for (int i = 0; i < ROOM_COUNT; i++) {
                    draw_room(rooms[i], i,player->level_floor);
                }
                break;
    }
    refresh();
    
}
void draw_drug_screen(Player *player, Room rooms[]) {
    clear(); // Clear the screen
    mvprintw(2, 5, "=== Player Status ===");
    
    // Draw health bar
    mvprintw(4, 5, "Portions: ");

    // Display food count
    mvprintw(6, 5, "SPEED PORTION: %d", player->drug[SPEED].count);
    mvprintw(9, 5, "DAMAGE PORTION: %d", player->drug[POWER].count);
    mvprintw(12, 5, "HEALTH PORTION: %d", player->drug[HEALTH].count);
    mvprintw(60, 5, "Press 'F' to go back");
    refresh();
    char ch;
    ch=getch(); 
    switch(ch){
        default:clear();
        for (int i = 0; i < ROOM_COUNT; i++) {
                    draw_room(rooms[i], i,player->level_floor);
                }
                break;
    }
    refresh();
    
}

void handle_input(char c, Player* player, Room rooms[]) {
    int n_x = player->xpos;
    int n_y = player->ypos;
    switch (c) {
        case 'k': case 'K': n_y--; break;
        case 'j': case 'J': n_y++; break;
        case 'l': case 'L': n_x++; break;
        case 'h': case 'H': n_x--; break;
        case 'u': case 'U': n_x++; n_y--; break;
        case 'y': case 'Y': n_x--; n_y--; break;
        case 'n': case 'N': n_x++; n_y++; break;
        case 'b': case 'B': n_x--; n_y++; break;
        case 'e': case 'E': draw_health_screen(player,rooms);break;
        case 'i': case 'I': draw_weapon_screen(player,rooms);break;
        case 't': case 'T': draw_drug_screen(player,rooms);break;
        
    }
    check_position(n_y, n_x, player,rooms);
}
void draw_health_bar(int y, int x, int health, int max_health) {
    int bar_width = 20; // Width of the health bar
    int filled = (health * bar_width) / max_health; // Calculate filled portion

    // Draw the health bar
    mvprintw(y, x, "Health: [");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            attron(COLOR_PAIR(2));
            addch('#'); // Filled portion
            attroff(COLOR_PAIR(2));
        } else {
            addch(' '); // Empty portion
        }
    }
    addch(']');
    mvprintw(y + 1, x, "%d/%d", health, max_health); // Display health value
}
int main_map() {
    
    srand(time(NULL));
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
    setlocale(LC_ALL, "");
    init_color(COLOR_ORANGE, 1000, 647, 0);
    init_color(COLOR_YELLOW, 1000, 800, 0);  // Custom yellow shade
    init_pair(1, COLOR_RED, COLOR_BLACK);     // Room 1
    init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Room 2
    init_pair(3, COLOR_BLUE, COLOR_BLACK);    // Room 3
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // Room 4
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    // Room 5
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // Room 6
    init_pair(7, COLOR_WHITE, COLOR_BLACK);   // Player
    init_pair(8, COLOR_ORANGE, COLOR_BLACK);
    init_pair(10, COLOR_YELLOW, COLOR_BLACK);
    map = malloc(MAP_HEIGHT * sizeof(char*));
    for(int i=0;i<MAP_HEIGHT;i++){
        *(map+i)=malloc(MAP_WIDTH*sizeof(char));
    }
    Room rooms[ROOM_COUNT];
    generate_rooms(rooms, ROOM_COUNT,1);

    for (int i = 0; i < ROOM_COUNT; i++) {
        draw_room(rooms[i], i, 1);
    }
    for(int i=0;i<5;i++){
        for(int t=0;t<24;t++){
        for(int f=0;f<80;f++){
            mark[t][f]=0;
        }
    }
        if(i==1){
            dfs(rooms[i].door_h_y,rooms[i].door_h_x,rooms[i].door_h_y,rooms[i].door_h_x,rooms[i+1].door_y,rooms[i+1].door_x);
        }
        else{
            dfs(rooms[i].door_y,rooms[i].door_x,rooms[i].door_y,rooms[i].door_x,rooms[i+1].door_y,rooms[i+1].door_x);
        }
    }
    //////monster
    Monster * demon;
    demon=setup_monster(rooms,ROOM_COUNT,'D',1);
    demon->health=5;
    Monster * fire=setup_monster(rooms,ROOM_COUNT,'F',2);
    fire->health=10;
    Monster * giant=setup_monster(rooms,ROOM_COUNT,'G',3);
    giant->health=15;
    Monster * snake=setup_monster(rooms,ROOM_COUNT,'S',4);
    snake->health=20;
    Monster * undeed=setup_monster(rooms,ROOM_COUNT,'U',5);
    undeed->health=30;
    demon->damage=10;
    fire->damage=15;
    giant->damage=20;
    snake->damage=25;
    undeed->damage=30;
    ///////////
    Player* player = setup_player(rooms, ROOM_COUNT);
    player->health=EASY_HEALTH;
    int health=player->health;
    player->curr_weapon=MACE;
    player->drug[0].count=0;
    player->drug[1].count=0;
    player->drug[2].count=0;
    ////////////
    player->a_num=0;
    player->gold=0;
    player->level_floor=1;
    player->food[0].count=0;
    player->weapon[0].count=1;
    player->hungriness=0;
    
    refresh();
    char input;
    
        while (player->level_floor==1 &&(input = getch()) != 'q') {
        if((player->xpos<rooms[1].x+rooms[1].width)&&(player->xpos>rooms[1].x)&&(player->ypos>rooms[1].y)&&(player->ypos<rooms[1].y+rooms[1].height)){
            player->health--;
        }
        draw_health_bar(0,40,player->health,health);
        if(reduce_health(player,demon)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<DEMON HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player,undeed)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<UNDEED HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player,snake)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<SNAKE HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player,giant)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<GIANT HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player,fire)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<FIRE MONSTER HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        global++;
        if(global%10==0){
            mvprintw(24,10,"              ");
            mvprintw(24,70,"                       ");
            mvprintw(25,60,"                                 ");
            mvprintw(26,70,"                       ");
        }
        //mvprintw(0,0,"HP:%d",player->health);
        mvprintw(24,0,"Gold:%d",player->gold);
        handle_input(input, player, rooms);
        if(input==' '){
            if(damage_enemies_melee(player,demon,player->curr_weapon)){
                mvprintw(25,60,"YOU HIT DEMON,DEMON CURRENT HEALTH:%d",demon->health);
            }
            else if(damage_enemies_melee(player,fire,player->curr_weapon)){
                mvprintw(25,60,"YOU HIT FIRE MONSTER,FIRE MONSTER CURRENT HEALTH:%d",fire->health);
            }
            else if(damage_enemies_melee(player,undeed,player->curr_weapon)){
                mvprintw(25,60,"YOU HIT UNDEED,UNDEED CURRENT HEALTH:%d",undeed->health);
            }
            else if(damage_enemies_melee(player,snake,player->curr_weapon)){
                mvprintw(25,60,"YOU HIT SNAKE,SNAKE CURRENT HEALTH:%d",snake->health);
            }
            else if(damage_enemies_melee(player,giant,player->curr_weapon)){
                mvprintw(25,60,"YOU HIT GIANT,GIANT CURRENT HEALTH:%d",giant->health);
            }
        }
        if(check_monster(demon)){
            mvprintw(26,70,"YOU KILLED DEMON");
                attron(COLOR_PAIR(5));
                mvprintw(demon->ypos,demon->xpos,".");
                attroff(COLOR_PAIR(5));
                map[demon->ypos][demon->xpos]='.';
        }
        else if(check_monster(fire)){
            mvprintw(26,70,"YOU KILLED FIRE MONSTER");
                attron(COLOR_PAIR(5));
                mvprintw(fire->ypos,fire->xpos,".");
                attroff(COLOR_PAIR(5));
                map[fire->ypos][fire->xpos]='.';
        }
        else if(check_monster(giant)){
            mvprintw(26,70,"YOU KILLED GIANT");
                attron(COLOR_PAIR(5));
                mvprintw(giant->ypos,giant->xpos,".");
                attroff(COLOR_PAIR(5));
                map[giant->ypos][giant->xpos]='.';
        }
        else if(check_monster(snake)){
            mvprintw(26,70,"YOU KILLED SNAKE");
                attron(COLOR_PAIR(5));
                mvprintw(snake->ypos,snake->xpos,".");
                attroff(COLOR_PAIR(5));
                map[snake->ypos][snake->xpos]='.';
        }
        else if(check_monster(undeed)){
            mvprintw(26,70,"YOU KILLED UNDEED");
            attron(COLOR_PAIR(5));
                mvprintw(undeed->ypos,undeed->xpos,".");
                attroff(COLOR_PAIR(5));
                map[undeed->ypos][undeed->xpos]='.';
        }
        if(player->health<=0){
            clear();
            attron(COLOR_PAIR(1));
            mvprintw(4,50,"GAME OVER");
            mvprintw(10,50,"YOU LOST.");
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(8));
            mvprintw(18,50,"TOTAL AMOUNT OF GOLD GAINED:%d",player->gold);
            refresh();
            char g=getchar();
            return 0;
        }
        player->hungriness++; 
        if(player->hungriness>=150){
            player->health--;
        }
        else if(player->hungriness<150 && player->health<(health-2)){
            player->health++;
        }
        refresh();
    }
    
    ///floor 2/////////////////////////
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        free(map[i]);
    }
    free(map);
    free(demon);
    free(fire);
    free(giant);
    free(undeed);
    free(snake);
    clear();
    map = malloc(MAP_HEIGHT * sizeof(char*));
    for(int i=0;i<MAP_HEIGHT;i++){
        *(map+i)=malloc(MAP_WIDTH*sizeof(char));
    }
    Room rooms2[ROOM_COUNT];
    generate_rooms(rooms2, ROOM_COUNT,2);

    for (int i = 0; i < ROOM_COUNT; i++) {
        draw_room(rooms2[i], i,2);
    }
    for(int i=0;i<5;i++){
        for(int t=0;t<24;t++){
        for(int f=0;f<80;f++){
            mark[t][f]=0;
        }
    }
        if(i==1){
            dfs(rooms2[i].door_h_y,rooms2[i].door_h_x,rooms2[i].door_h_y,rooms2[i].door_h_x,rooms2[i+1].door_y,rooms2[i+1].door_x);
        }
        else{
            dfs(rooms2[i].door_y,rooms2[i].door_x,rooms2[i].door_y,rooms2[i].door_x,rooms2[i+1].door_y,rooms2[i+1].door_x);
        }
    }
    ///////////////monster

    demon=setup_monster(rooms2,ROOM_COUNT,'D',1);
    demon->health=5;
    fire=setup_monster(rooms2,ROOM_COUNT,'F',2);
    fire->health=10;
    giant=setup_monster(rooms2,ROOM_COUNT,'G',3);
    giant->health=15;
    snake=setup_monster(rooms2,ROOM_COUNT,'S',4);
    snake->health=20;
    undeed=setup_monster(rooms2,ROOM_COUNT,'U',5);
    undeed->health=30;
    ////////////
    Player* player2 = setup_player(rooms2, ROOM_COUNT);
    player2->health=player->health;
    player2->a_num=player->a_num;
    player2->curr_weapon=player->curr_weapon;
    player2->level_floor=2;
    player2->food[0].count=player->food[0].count;
    //////////
    player2->weapon[0].count=player->weapon[0].count;
    player2->weapon[1].count=player->weapon[1].count;
    player2->weapon[2].count=player->weapon[2].count;
    player2->weapon[3].count=player->weapon[3].count;
    player2->weapon[4].count=player->weapon[4].count;
    /////////////
    player2->drug[0].count=player->drug[0].count;
    player2->drug[1].count=player->drug[1].count;
    player2->drug[2].count=player->drug[2].count;
    ///////////
    player2->hungriness=player->hungriness;
    free(player);
        while (player2->level_floor==2 && (input = getch()) != 'q') {
            if((player2->xpos<rooms2[1].x+rooms2[1].width)&&(player2->xpos>rooms2[1].x)&&(player2->ypos>rooms2[1].y)&&(player2->ypos<rooms2[1].y+rooms2[1].height)){
            player2->health--;
        }
            draw_health_bar(0,40,player2->health,health);
            if(reduce_health(player2,demon)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<DEMON HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player2,undeed)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<UNDEED HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player2,snake)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<SNAKE HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player2,giant)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<GIANT HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player2,fire)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<FIRE MONSTER HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        global++;
        if(global%10==0){
            mvprintw(24,10,"              ");
            mvprintw(24,70,"                       ");
        }
        if(player2->health<=0){
            clear();
            attron(COLOR_PAIR(1));
            mvprintw(4,50,"GAME OVER");
            mvprintw(10,50,"YOU LOST.");
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(8));
            mvprintw(18,50,"TOTAL AMOUNT OF GOLD GAINED:%d",player2->gold);
            refresh();
            char g=getchar();
            return 0;
        }
        //mvprintw(0,0,"HP:%d",player2->health);
        mvprintw(24,0,"Gold:%d",player2->gold);
        handle_input(input, player2, rooms2); 
        player2->hungriness++; 
        if(player2->hungriness>=50){
            player2->health--;
        }
        else if(player2->hungriness<150 && player2->health<(health-2)){
            player2->health++;
        }
        refresh();
    }
    ///////
    ///////////floor 3
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        free(map[i]);
    }
    free(map);
    free(demon);
    free(fire);
    free(giant);
    free(undeed);
    free(snake);
    clear();
    map = malloc(MAP_HEIGHT * sizeof(char*));
    for(int i=0;i<MAP_HEIGHT;i++){
        *(map+i)=malloc(MAP_WIDTH*sizeof(char));
    }
    Room rooms3[ROOM_COUNT];
    generate_rooms(rooms3, ROOM_COUNT,3);

    for (int i = 0; i < ROOM_COUNT; i++) {
        draw_room(rooms3[i], i, 3);
    }
    for(int i=0;i<5;i++){
        for(int t=0;t<24;t++){
        for(int f=0;f<80;f++){
            mark[t][f]=0;
        }
    }
        if(i==1){
            dfs(rooms3[i].door_h_y,rooms3[i].door_h_x,rooms3[i].door_h_y,rooms3[i].door_h_x,rooms3[i+1].door_y,rooms3[i+1].door_x);
        }
        else{
            dfs(rooms3[i].door_y,rooms3[i].door_x,rooms3[i].door_y,rooms3[i].door_x,rooms3[i+1].door_y,rooms3[i+1].door_x);
        }
    }
    ///////////////monster
    demon=setup_monster(rooms3,ROOM_COUNT,'D',1);
    demon->health=5;
    fire=setup_monster(rooms3,ROOM_COUNT,'F',2);
    fire->health=10;
    giant=setup_monster(rooms3,ROOM_COUNT,'G',3);
    giant->health=15;
    snake=setup_monster(rooms3,ROOM_COUNT,'S',4);
    snake->health=20;
    undeed=setup_monster(rooms3,ROOM_COUNT,'U',5);
    undeed->health=30;
    ////////////
    Player* player3 = setup_player(rooms3, ROOM_COUNT);
    player3->health=player2->health;
    player3->a_num=player2->a_num;
    player3->curr_weapon=player2->curr_weapon;
    player3->level_floor=3;
    player3->food[0].count=player2->food[0].count;
    player3->weapon[0].count=player2->weapon[0].count;
    //////////
    player3->weapon[1].count=player2->weapon[1].count;
    player3->weapon[2].count=player2->weapon[2].count;
    player3->weapon[3].count=player2->weapon[3].count;
    player3->weapon[4].count=player2->weapon[4].count;
    /////////////
    player3->drug[0].count=player2->drug[0].count;
    player3->drug[1].count=player2->drug[1].count;
    player3->drug[2].count=player2->drug[2].count;
    ///////////
    player3->hungriness=player2->hungriness;
    free(player2);
        while (player3->level_floor==3 && (input = getch()) != 'q') {
            if((player3->xpos<rooms3[1].x+rooms3[1].width)&&(player3->xpos>rooms3[1].x)&&(player3->ypos>rooms3[1].y)&&(player3->ypos<rooms3[1].y+rooms3[1].height)){
            player3->health--;
        }
            draw_health_bar(0,40,player3->health,health);
            if(reduce_health(player3,demon)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<DEMON HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player3,undeed)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<UNDEED HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player3,snake)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<SNAKE HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player3,giant)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<GIANT HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player3,fire)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<FIRE MONSTER HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        global++;
        if(global%10==0){
            mvprintw(24,10,"              ");
            mvprintw(24,70,"                          ");
        }
        if(player3->health<=0){
            clear();
            attron(COLOR_PAIR(1));
            mvprintw(4,50,"GAME OVER");
            mvprintw(10,50,"YOU LOST.");
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(8));
            mvprintw(18,50,"TOTAL AMOUNT OF GOLD GAINED:%d",player3->gold);
            refresh();
            char g=getchar();
            return 0;
        }
        //mvprintw(0,0,"HP:%d",player3->health);
        mvprintw(24,0,"Gold:%d",player3->gold);
        handle_input(input, player3, rooms3);
        player3->hungriness++; 
        if(player3->hungriness>=50){
            player3->health--;
        }
        else if(player3->hungriness<150 && player3->health<(health-2)){
            player3->health+=3;
        }
        refresh();
    }
    ///////////////
    /////floor 4
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        free(map[i]);
    }
    free(map);
    free(demon);
    free(fire);
    free(giant);
    free(undeed);
    free(snake);
    clear();
    map = malloc(MAP_HEIGHT * sizeof(char*));
    for(int i=0;i<MAP_HEIGHT;i++){
        *(map+i)=malloc(MAP_WIDTH*sizeof(char));
    }
    Room rooms4[ROOM_COUNT];
    generate_rooms(rooms4, ROOM_COUNT,4);

    for (int i = 0; i < ROOM_COUNT; i++) {
        draw_room(rooms4[i], i, 4);
    }
    for(int i=0;i<5;i++){
        for(int t=0;t<24;t++){
        for(int f=0;f<80;f++){
            mark[t][f]=0;
        }
    }
        if(i==1){
            dfs(rooms4[i].door_h_y,rooms4[i].door_h_x,rooms4[i].door_h_y,rooms4[i].door_h_x,rooms4[i+1].door_y,rooms4[i+1].door_x);
        }
        else{
            dfs(rooms4[i].door_y,rooms4[i].door_x,rooms4[i].door_y,rooms4[i].door_x,rooms4[i+1].door_y,rooms4[i+1].door_x);
        }
    }
    //////monster
    demon=setup_monster(rooms4,ROOM_COUNT,'D',1);
    demon->health=5;
    fire=setup_monster(rooms4,ROOM_COUNT,'F',2);
    fire->health=10;
    giant=setup_monster(rooms4,ROOM_COUNT,'G',3);
    giant->health=15;
    snake=setup_monster(rooms4,ROOM_COUNT,'S',4);
    snake->health=20;
    undeed=setup_monster(rooms4,ROOM_COUNT,'U',5);
    undeed->health=30;
    ///////////
    Player* player4 = setup_player(rooms4, ROOM_COUNT);
    player4->health=player3->health;
    player4->a_num=player3->a_num;
    player4->curr_weapon=player3->curr_weapon;
    player4->level_floor=4;
    player4->food[0].count=player3->food[0].count;
    player4->weapon[0].count=player3->weapon[0].count;
    //////////
    player4->weapon[1].count=player3->weapon[1].count;
    player4->weapon[2].count=player3->weapon[2].count;
    player4->weapon[3].count=player3->weapon[3].count;
    player4->weapon[4].count=player3->weapon[4].count;
    //////////
    player4->drug[0].count=player3->drug[0].count;
    player4->drug[1].count=player3->drug[1].count;
    player4->drug[2].count=player3->drug[2].count;
    ///////////
    player4->hungriness=player3->hungriness;
    free(player3);
        while (player4->level_floor==4 && (input = getch()) != 'q') {
            if((player4->xpos<rooms4[1].x+rooms4[1].width)&&(player->xpos>rooms4[1].x)&&(player->ypos>rooms4[1].y)&&(player->ypos<rooms4[1].y+rooms4[1].height)){
            player4->health--;
        }
            draw_health_bar(0,40,player4->health,health);
            if(reduce_health(player4,demon)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<DEMON HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player4,undeed)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<UNDEED HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player4,snake)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<SNAKE HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player4,giant)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<GIANT HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        else if(reduce_health(player4,fire)){
            attron(COLOR_ORANGE);
            mvprintw(24,70,"<FIRE MONSTER HIT YOU>");
            attroff(COLOR_ORANGE);
        }
        global++;
        if(global%10==0){
            mvprintw(24,10,"              ");
            mvprintw(24,70,"                        ");
        }
        if(player4->health<=0){
            clear();
            attron(COLOR_PAIR(1));
            mvprintw(4,50,"GAME OVER");
            mvprintw(10,50,"YOU LOST.");
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(8));
            mvprintw(18,50,"TOTAL AMOUNT OF GOLD GAINED:%d",player4->gold);
            refresh();
            char g=getchar();
            return 0;
        }
        //mvprintw(0,0,"HP:%d",player4->health);
        mvprintw(24,0,"Gold:%d",player4->gold);
        handle_input(input, player4, rooms4);
        player4->hungriness++; 
        if(player4->hungriness>=50){
            player4->health--;
        }
        else if(player4->hungriness<150 && player4->health<(health-2)){
            player4->health+=3;
        }
        refresh();
    }
    ///////////////

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        free(map[i]);
    }
    free(map);
    free(demon);
    free(fire);
    free(giant);
    free(undeed);
    free(snake);
    free(player4);
    endwin();
    return 0;
}
#endif map_h