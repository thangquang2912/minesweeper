#include "function.h"
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
void push(queue<pair<int, int>> &q, vector<vector<bool>> &visit, int m, int n, vector<vector<char>> &board)
{
    if (m - 1 >= 0 && visit[m - 1][n] == false && board[m - 1][n] != 'M')
    {
        visit[m - 1][n] = true;
        q.push({m - 1, n});
    }
    if (m + 1 < board.size() && visit[m + 1][n] == false && board[m + 1][n] != 'M')
    {
        visit[m + 1][n] = true;
        q.push({m + 1, n});
    }
    if (n - 1 >= 0 && visit[m][n - 1] == false && board[m][n - 1] != 'M')
    {
        visit[m][n - 1] = true;
        q.push({m, n - 1});
    }
    if (n + 1 < board[0].size() && visit[m][n + 1] == false && board[m][n + 1] != 'M')
    {
        visit[m][n + 1] = true;
        q.push({m, n + 1});
    }
}
vector<vector<char>> updateBoard(vector<vector<char>> &board, vector<int> &click, vector<vector<bool>> &visit)
{
    visit[click[0]][click[1]] = true;
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
    if (boardToDisplay[click[0]][click[1]] != 'B' || (boardToDisplay[click[0]][click[1]] > '0' && boardToDisplay[click[0]][click[1]] < '9'))
    {
        boardToDisplay[click[0]][click[1]] = 'F';
    }
}
void copyBoardToDisplay(vector<vector<char>> &boardToDisplay, vector<vector<char>> board, vector<vector<bool>> visit)
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            if (board[i][j] != 'E' )
            {
                if(!(board[i][j] == 'M' && !visit[i][j])){
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
            menuLevel();
            bool isCancel = false;
            int rows = 0;
            int cols = 0;
            int numsOfBomb = 0;
            while (!isCancel)
            {
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
                    isCancel = true;
                    break;
                }
                case 2:
                {
                    rows = rowsMediumLevel;
                    cols = colsMediumLevel;
                    numsOfBomb = numsOfBombInMediumLevel;
                    isCancel = true;
                    break;
                }
                case 3:
                {
                    rows = rowsHardLevel;
                    cols = colsHardLevel;
                    numsOfBomb = numsOfBombInHardLevel;
                    isCancel = true;
                    break;
                }
                default:
                {
                    cout << "You have already entered a wrong choice! Please enter again!!!\n";
                    break;
                }
                }
            }
            if (rows == 0)
            {
                break;
            }
            bool isFirst = true;
            vector<vector<char>> boardToDisplay(rows, vector<char>(cols, 'E'));
            vector<vector<bool>> visit(rows, vector<bool>(cols, false));
            vector<vector<char>> board(rows, vector<char>(cols, 'E'));
            while (count('E', boardToDisplay) + count('F', boardToDisplay) > numsOfBomb)
            {
                printWhenPlay(boardToDisplay, numsOfBomb);
                int choice;
                cout << "Enter your choice: ";
                cin >> choice;
                switch (choice)
                {
                case 0:
                {
                    // maybe add save in this
                    numsOfBomb = INT_MAX;
                    break;
                }
                case 1:
                {
                    boardToDisplay = vector<vector<char>>(rows, vector<char>(cols, 'E'));
                    visit = vector<vector<bool>>(rows, vector<bool>(cols, false));
                    break;
                }
                case 2:
                {
                    vector<int> click;
                    pair<int, int> p;
                    cout << "Enter your square you want to set flag: ";
                    cin >> p.first >> p.second;
                    click.push_back(p.first);
                    click.push_back(p.second);
                    setFlag(boardToDisplay, click);
                    break;
                }
                case 3:
                {
                    vector<int> click;
                    pair<int, int> p;
                    cout << "Enter your square you want to open: ";
                    cin >> p.first >> p.second;
                    click.push_back(p.first);
                    click.push_back(p.second);
                    if (board[click[0]][click[1]] == 'M')
                    {
                        clearScreen();
                        cout << "Bummm....\n";
                        Sleep(10);
                        clearScreen();
                        cout << "End Game!\n";
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