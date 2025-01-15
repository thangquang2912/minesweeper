#include "function.h"

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
// The path to save the game after it was abruptly close
string pathToSave = "DataUser/username/gameSave";


void login(){
    
}

void clearScreen()
{
    system("cls");
}
void menuGame()
{
    cout << "0. Exit\n";
    cout << "1. New Game\n";
    cout << "2. Highscores\n";
    cout << "3. Play continue\n";
    cout << "4. Option\n";
}
void menuOption()
{
    cout << "0. Exit\n";
    cout << "1. Country\n";
    cout << "2. My Highscores\n";
    cout << "3. About\n";
    cout << "4. Help\n";
}
void menuWhenPlay()
{
    cout << "| 0. Exit |" << " 1. Restart game |" << " 2. Flag |" << " 3. Enter your square to open |\n";
}
void menuLevel()
{
    cout << "0. Exit\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
}
void printBoard(vector<vector<char>> board)
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            cout << board[i][j] << " | ";
        }
        cout << endl;
        for (int j = 0; j < board[0].size(); j++)
        {
            cout << " -  ";
        }
        cout << endl;
    }
}
int count(char c, vector<vector<char>> board)
{
    int cnt = 0;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            if (board[i][j] == c || (board[i][j] == 'F' && c == 'M'))
            {
                cnt++;
            }
        }
    }
    return cnt;
}
void printWhenPlay(vector<vector<char>> boardToDisplay, int numsOfBomb)
{
    cout << "So bom: " << numsOfBomb - count('F', boardToDisplay) << "\t";
    cout << endl;
    printBoard(boardToDisplay);
    cout << endl;
    menuWhenPlay();
}
void printWhenEndGame(vector<vector<char>> boardToDisplay, vector<vector<char>> board, int numsOfBomb){
    cout << "So bom: " << numsOfBomb - count('F', boardToDisplay) << "\t";
    cout << endl;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            if(board[i][j] != 'M' && boardToDisplay[i][j] == 'F'){
                boardToDisplay[i][j] = 'W';
            }
            if(board[i][j] == 'M' && boardToDisplay[i][j] != 'F'){
                boardToDisplay[i][j] = 'M';
            }
        }
    }
    printBoard(boardToDisplay);
    cout << endl;
}
int countBomb(vector<vector<char>> &board, int m, int n)
{
    int count = 0;
    for (int i = m - 1; i <= m + 1; i++)
    {
        if (i >= 0 && i < board.size())
        {
            for (int j = n - 1; j <= n + 1; j++)
            {
                if (j >= 0 && j < board[0].size())
                {
                    if (board[i][j] == 'M')
                    {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}
void push(queue<pair<int, int>> &q, vector<vector<char>> &visit, int m, int n, vector<vector<char>> &board)
{
    for (int i = m - 1; i <= m + 1; i++) {
        if (i >= 0 && i < board.size()) {
            for (int j = n - 1; j <= n + 1; j++) {
                if (j >= 0 && j < board[0].size()) {
                    if(visit[i][j] == '0' && board[i][j] != 'M'){
                        visit[i][j] = '1';
                        q.push({i, j});
                    }
                }
            }
        }
    }
}
vector<vector<char>> updateBoard(vector<vector<char>> &board, vector<int> &click, vector<vector<char>> &visit)
{
    visit[click[0]][click[1]] = '1';
    queue<pair<int, int>> q;
    if (board[click[0]][click[1]] == 'M')
    {
        board[click[0]][click[1]] = 'X';
        return board;
    }
    q.push({click[0], click[1]});
    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();
        int cnt = countBomb(board, p.first, p.second);
        if (cnt != 0)
        {
            board[p.first][p.second] = char(cnt + 48);
        }
        else
        {
            board[p.first][p.second] = 'B';
            push(q, visit, p.first, p.second, board);
        }
    }
    return board;
}

vector<pair<int, int>> createAdjacent(int i, int j, vector<vector<char>> board)
{
    vector<pair<int, int>> adjacent;
    for (int k = i - 1; k <= i + 1; k++)
    {
        if (k >= 0 && k < board.size())
        {
            for (int l = j - 1; l <= j + 1; l++)
            {
                if (l >= 0 && l < board[0].size())
                {
                    adjacent.push_back({k, l});
                }
            }
        }
    }
    return adjacent;
}
bool checkToSetBombs(int i, int j, vector<pair<int, int>> adjacent)
{
    for (int m = 0; m < adjacent.size(); m++)
    {
        if (i == adjacent[m].first && j == adjacent[m].second)
        {
            return true;
        }
    }
    return false;
}
vector<vector<char>> createBoard(int numsOfBomb, vector<int> click, int rows, int cols)
{
    srand(time(0));
    vector<vector<char>> b(rows, vector<char>(cols, 'E'));
    b[click[0]][click[1]] = 'B';
    vector<pair<int, int>> adjacent = createAdjacent(click[0], click[1], b);
    while (count('M', b) != numsOfBomb)
    {
        int i = rand() % rows;
        int j = rand() % cols;
        if (!checkToSetBombs(i, j, adjacent))
        {
            b[i][j] = 'M';
        }
    }
    return b;
}
void setFlag(vector<vector<char>> &boardToDisplay, vector<int> click)
{
    if (boardToDisplay[click[0]][click[1]] != ' ' || (boardToDisplay[click[0]][click[1]] > '0' && boardToDisplay[click[0]][click[1]] < '9'))
    {
        boardToDisplay[click[0]][click[1]] = 'F';
    }
}
void unSetFlag(vector<vector<char>> &boardToDisplay, vector<int> click)
{
    if (boardToDisplay[click[0]][click[1]] != ' ' || (boardToDisplay[click[0]][click[1]] > '0' && boardToDisplay[click[0]][click[1]] < '9'))
    {
        boardToDisplay[click[0]][click[1]] = 'E';
    }
}
void copyBoardToDisplay(vector<vector<char>> &boardToDisplay, vector<vector<char>> board, vector<vector<char>> visit)
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            if (board[i][j] != 'E' )
            {
                if(!(board[i][j] == 'M' && visit[i][j] == '0')){
                    if (board[i][j] == 'B')
                    {
                        boardToDisplay[i][j] = ' ';
                    }
                    else
                    {
                        boardToDisplay[i][j] = board[i][j];
                    }
                }
                
            }
        }
    }
}

void readBoardToFile(vector<vector<char>> boardToDisplay, string level, string fileName){
    ofstream ofs;
    ofs.open(fileName);
    ofs << level << endl;
    string row{};
    string col{};
    row = "rows" + level;
    col = "cols" + level;
    for(int i = 0; i < stoi(row); i++){
        for(int j = 0; j < stoi(col); j++){
            ofs << boardToDisplay[i][j] << " ";
        }
        ofs << endl;
    }
    ofs.close();
}
void extractBoardFromFile(vector<vector<char>>& boardToDisplay, string& level, string fileName){
    ifstream ifs;
    ifs.open(fileName);
    if(!ifs.is_open()){
        cout << "Mo file that bai\n";
        return;
    }
    int k = 0;
    while(ifs.good() && k < 5){
        for (int i = 0; i < 5; i++)
        {
            string tmp{};
            if (i == 5 - 1)
            {
                getline(ifs, tmp);
                boardToDisplay[k++][i] = tmp[0];
                break;
            }
            getline(ifs, tmp, ' ');
            boardToDisplay[k][i] = tmp[0];
        }
    }
    ifs.close();
}
// Ham lon
void playGame()
{
    // Dat co de quan ly viec choi va thoat khoi game
    bool flag = true;
    while (flag)
    {
        // tao menu lua chon cho user nhap vao
        menuGame();
        int choose;
        cout << "Enter your choice: ";
        cin >> choose;
        clearScreen();
        switch (choose)
        {
        // exit
        case 0:
        {
            flag = false;
            break;
        }
        // New Game
        case 1:
        {
            bool isCancel = false;
            int rows = 0;
            int cols = 0;
            int numsOfBomb = 0;
            string levelToUseInFile{};
            while (!isCancel)
            {
                menuLevel();
                int level;
                cout << "Enter your level you want to play: ";
                cin >> level;
                switch (level)
                {
                case 0:
                {
                    isCancel = true;
                    break;
                }
                case 1:
                {
                    rows = rowsEasyLevel;
                    cols = colsEasyLevel;
                    numsOfBomb = numsOfBombInEasyLevel;
                    levelToUseInFile = "EasyLevel";
                    isCancel = true;
                    break;
                }
                case 2:
                {
                    rows = rowsMediumLevel;
                    cols = colsMediumLevel;
                    numsOfBomb = numsOfBombInMediumLevel;
                    levelToUseInFile = "MediumLevel";
                    isCancel = true;
                    break;
                }
                case 3:
                {
                    rows = rowsHardLevel;
                    cols = colsHardLevel;
                    numsOfBomb = numsOfBombInHardLevel;
                    levelToUseInFile = "HardLevel";
                    isCancel = true;
                    break;
                }
                default:
                {
                    clearScreen();
                    cout << "You have already entered a wrong choice! Please enter again!!!\n";
                    break;
                }
                }
            }
            clearScreen();
            if (rows == 0)
            {
                break;
            }
            bool isFirst = true;
            vector<vector<char>> boardToDisplay(rows, vector<char>(cols, 'E'));
            vector<vector<char>> visit(rows, vector<char>(cols, '0'));
            vector<vector<char>> board(rows, vector<char>(cols, 'E'));
            while (numsOfBomb != INT_MAX)
            {
                printWhenPlay(boardToDisplay, numsOfBomb);
                int choice;
                cout << "Enter your choice: ";
                cin >> choice;
                switch (choice)
                {
                case 0:
                {
                    if(!isFirst){
                        clearScreen();
                        cout << "Do you really want to exit this game?(Y/N): \n";
                        string isExit{};
                        cin >> isExit;
                        if(isExit == "N"){
                            break;
                        }else{
                            isFirst = true;
                        }
                    }
                    // maybe add save in this
                    numsOfBomb = INT_MAX;
                    break;
                }
                case 1:
                {
                    if(!isFirst){
                        clearScreen();
                        cout << "Do you really want to restart this game?(Y/N): \n";
                        string isRestart{};
                        cin >> isRestart;
                        if(isRestart == "N"){
                            break;
                        }else{
                            isFirst = true;
                        }
                    }
                    vector<vector<char>> board(rows, vector<char>(cols, 'E'));
                    boardToDisplay = vector<vector<char>>(rows, vector<char>(cols, 'E'));
                    visit = vector<vector<char>>(rows, vector<char>(cols, '0'));
                    break;
                }
                case 2:
                {
                    vector<int> click;
                    pair<int, int> p;
                    cout << "Enter your square you want to set flag: ";
                    cin >> p.first >> p.second;
                    while(p.first < 0 || p.second < 0 || p.first >= rows || p.second >= cols || (boardToDisplay[p.first][p.second] > '0' && boardToDisplay[p.first][p.second] < '9') || boardToDisplay[p.first][p.second] == ' '){
                        cout << "You have already entered wrong choice. Please enter again!\n";
                        cout << "Enter your square you want to set flag: ";
                        cin >> p.first >> p.second;
                    }
                    click.push_back(p.first);
                    click.push_back(p.second);
                    clearScreen();
                    if(boardToDisplay[click[0]][click[1]] == 'F'){
                        unSetFlag(boardToDisplay, click);
                    }else{
                        setFlag(boardToDisplay, click);
                    }
                    if(count('E', boardToDisplay) + count('F', boardToDisplay) == numsOfBomb){
                        clearScreen();
                        printWhenEndGame(boardToDisplay, board, numsOfBomb);
                        cout << endl << "You are win!\n";
                        cout << "Press any key to return the menu game: ";
                        cin.ignore();
                        cin.get();
                        numsOfBomb = INT_MAX;
                        break;
                    }
                    break;
                }
                case 3:
                {
                    vector<int> click;
                    pair<int, int> p;
                    cout << "Enter your square you want to open: ";
                    cin >> p.first >> p.second;
                    while(p.first < 0 || p.second < 0 || p.first >= rows || p.second >= cols || boardToDisplay[p.first][p.second] == 'F' || (boardToDisplay[p.first][p.second] > '0' && boardToDisplay[p.first][p.second] < '9') || boardToDisplay[p.first][p.second] == ' '){
                        cout << "You have already entered wrong choice. Please enter again!\n";
                        cout << "Enter your square you want to open: ";
                        cin >> p.first >> p.second;
                    }
                    click.push_back(p.first);
                    click.push_back(p.second);
                    clearScreen();
                    if (board[click[0]][click[1]] == 'M')
                    {
                        board = updateBoard(board, click, visit);
                        copyBoardToDisplay(boardToDisplay, board, visit);
                        clearScreen();
                        cout << "Bummm....\n";
                        Sleep(50);
                        clearScreen();
                        printWhenEndGame(boardToDisplay, board, numsOfBomb);
                        cout << "End Game!\n";
                        cout << "Press any key to return the menu game: ";
                        cin.ignore();
                        cin.get();
                        clearScreen();
                        numsOfBomb = INT_MAX;
                        break;
                    }
                    if (isFirst)
                    {
                        board = createBoard(numsOfBomb, click, rows, cols);
                        isFirst = false;
                    }
                    board = updateBoard(board, click, visit);
                    copyBoardToDisplay(boardToDisplay, board, visit);
                    if(count('E', boardToDisplay) + count('F', boardToDisplay) == numsOfBomb ){
                        clearScreen();
                        printWhenEndGame(boardToDisplay, board, numsOfBomb);
                        cout << endl << "You are win!\n";
                        cout << "Press any key to return the menu game: ";
                        cin.ignore();
                        cin.get();
                        clearScreen();
                        numsOfBomb = INT_MAX;
                        break;
                    }
                    break;
                }
                }
            }
            
            break;
        }
        case 2:
        {
        }
        case 3:
        {
        }
        case 4:
        {
        }
        }
    }
}