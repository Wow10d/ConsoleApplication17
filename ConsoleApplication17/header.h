#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <fstream>

#define ROAD_LEN 21
#define MIN_X 1
#define MAX_X 60
#define CAR_Y 18
#define KEY_SPACE 32
#define LB_SIZE 5

enum ConsoleColor
{
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray,
	LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};

struct lbEntry {
    char name[16];
    int score;
	int time;
};
using namespace std;

void showCursor(bool flag);
void setCursorPosition(COORD coord);
void SetColor(int, int);
void move_road(COORD* dx, COORD road[2][ROAD_LEN], COORD trouble[ROAD_LEN],COORD gifts[ROAD_LEN], bool);
bool check_r(COORD car, COORD road[2][ROAD_LEN], COORD trouble[ROAD_LEN], COORD gifts[ROAD_LEN], int&);
void printCarGame();
void loadLeaderboard(lbEntry* arr, const char* filename);
void saveLeaderboard(lbEntry* arr, const char* filename);
void addToLeaderboard(lbEntry* arr, const char* name, int score, int);
void printLeaderboard(lbEntry* arr, const char* title, short x, short y);