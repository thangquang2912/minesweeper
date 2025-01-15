#ifndef function_h
#define function_h
#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<fstream>
#include<cstring>
#include<string>
#include <windows.h>
using namespace std;
struct user{
    string username;
    string password;
    string nameInGame;
};



void clearScreen();
void playGame();
#endif