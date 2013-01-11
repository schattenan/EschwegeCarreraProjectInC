//#include "string.h"
#include "stdbool.h"
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>

void gotoxy(int x, int y);
bool confirm();
void initUI();
void getName(char name[42]);
void buildTable(char playername1[42],char playername2[42],char playername3[42],char playername4[42]); 