#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<string>
#include<ctime>
#include<iomanip>
#include"1SV2K.h"
#define NUM_MAXMAPNUM 3
#define NUM_MAXDIFFICULTY 3
#define NUM_MAXPLAYERTYPE 3
#define NUM_MAXABILITYNUM 3
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;
HANDLE hOut;
COORD posBegin={20,7},posPause={21,9};


int main()
{
	system("title 某人的欢乐小游戏:\"一熊战二狗\"");
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	restart:
	system("color F0");
	system("cls");
	SetConsoleCursorPosition(hOut,posBegin);
	printf("1 Survivor VS 2 Killers");
	SetConsoleCursorPosition(hOut,posPause);
	system("pause");
	if(KEY_DOWN(VK_ESCAPE))return 0;
	offlineSurvivor::offlineSurvivor();
	goto restart;
	return 0;
}
