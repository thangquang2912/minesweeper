#ifndef function_h
#define function_h
#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

// CONSTANT
// Easy Level
int rowsEasyLevel = 5;
int colsEasyLevel = 5;
int numsOfBombInEasyLevel = 7;
// Medium Level
int rowsMediumLevel = 10;
int colsMediumLevel = 10;
int numsOfBombInMediumLevel = 40;
// Hard Level
int rowsHardLevel = 23;
int colsHardLevel = 11;
int numsOfBombInHardLevel = 95;
// Save The Game Before
vector<vector<char>> tmpBoard{};
vector<vector<bool>> tmpVisit{};
vector<vector<char>> tmpBoardToDisplay{};
bool tmpIsFirst = false;


void clearScreen();
void playGame();
#endif