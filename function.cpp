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

void menuLogin()
{
    cout << "1. Sign in\n";
    cout << "2. Sign up\n";
}

void clearScreen()
{
    system("cls");
}
//dat flag de xem co continue khong
void menuGame(user u)
{
    cout << setw(30) << "user:" << u.username << endl;
    cout << "0. Sign out\n";
    cout << "1. New Game\n";
    cout << "2. Highscores\n";
    cout << "3. Option\n";
    cout << "4. Play continue\n";
}
void menuGameNotCon(user u){
    cout << setw(30) << "user:" << u.username << endl;
    cout << "0. Sign out\n";
    cout << "1. New Game\n";
    cout << "2. Highscores\n";
    cout << "3. Option\n";
}
void menuOption(user u)
{
    cout << setw(30) << u.username;
    cout << "0. Back\n";
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
void printWhenEndGame(vector<vector<char>> boardToDisplay, vector<vector<char>> board, int numsOfBomb)
{
    cout << "So bom: " << numsOfBomb - count('F', boardToDisplay) << "\t";
    cout << endl;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            if (board[i][j] != 'M' && boardToDisplay[i][j] == 'F')
            {
                boardToDisplay[i][j] = 'W';
            }
            if (board[i][j] == 'M' && boardToDisplay[i][j] != 'F')
            {
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
    for (int i = m - 1; i <= m + 1; i++)
    {
        if (i >= 0 && i < board.size())
        {
            for (int j = n - 1; j <= n + 1; j++)
            {
                if (j >= 0 && j < board[0].size())
                {
                    if (visit[i][j] == '0' && board[i][j] != 'M')
                    {
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
            if (board[i][j] != 'E')
            {
                if (!(board[i][j] == 'M' && visit[i][j] == '0'))
                {
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

void writeBoardToFile(vector<vector<char>> board, string level, string rows, string cols, string numsOfBomb, string fileName)
{
    ofstream ofs;
    ofs.open(fileName);
    ofs << level << endl;
    ofs << rows << " " << cols << " " << numsOfBomb << endl;
    for (int i = 0; i < stoi(rows); i++)
    {
        for (int j = 0; j < stoi(cols); j++)
        {
            if(j != stoi(cols) - 1){
                ofs << board[i][j] << ",";
            }else{
                ofs << board[i][j];
            }
            
        }
        if(i < stoi(rows) - 1){
            ofs << endl;
        }
    }
    ofs.close();
}
void extractBoardFromFile(vector<vector<char>> &board, string &level, string fileName, int& rows, int& cols, int& numsOfBomb)
{
    ifstream ifs;
    ifs.open(fileName);
    if (!ifs.is_open())
    {
        cout << "Mo file that bai\n";
        return;
    }
    getline(ifs, level);
    string row{};
    string col{};
    string b{};
    getline(ifs, row, ' ');
    getline(ifs, col, ' ');
    getline(ifs, b);
    rows = stoi(row);
    cols = stoi(col);
    numsOfBomb = stoi(b);
    int k = 0;
    board = vector<vector<char>>(rows, vector<char>(cols));
    while (!ifs.eof() && k < rows)
    {
        for (int i = 0; i < cols; i++)
        {
            string tmp{};
            if (i == cols - 1)
            {
                getline(ifs, tmp);
                board[k++][i] = tmp[0];
                break;
            }
            getline(ifs, tmp, ',');
            board[k][i] = tmp[0];
        }
    }
    ifs.close();
}
void writeTimeToFile(int h, int m, int s, string fileName)
{
    ofstream ofs;
    ofs.open(fileName, ios::app);
    string hours = to_string(h);
    string minutes = to_string(m);
    string seconds = to_string(s);
    ofs << hours << ":" << minutes << ":" << seconds << "\n";
    ofs.close();
}
bool checkPassWord(string password){
    if(password.size() < 8){
        return false;
    }
    for(int i = 0; i < password.size(); i++){
        if(!(islower(password[i]) || isupper(password[i]) || isdigit(password[i]) || password[i] != '\b')){
            return false;
        }
    }
    return true;
}
bool checkUsername(string username){
    if(username.size() < 8 || username.size() > 20){
        return false;
    }
    for(int i = 0; i < username.size(); i++){
        if(!(islower(username[i]) || isupper(username[i]) || isdigit(username[i]) || username[i] != '\b')){
            return false;
        }
    }
    return true;
}
string hiddenPass(){
    string pass{};
    char c;
    while(true){
        c = _getch();
        if(c == '\r'){
            cout << endl;
            break;
        }
        if(c == '\b'){
            if(!pass.empty()){
                pass.pop_back();
            }
            cout << "\b";
        }else{
            pass += c;
            cout << "*";
        }
        
    }
    return pass;
}
void writeUsersToFile(user u){
    ofstream ofs;
    ofs.open("DataUsers/ListUsers.csv", ios::app);
    ifstream ifs;
    ifs.open("DataUsers/ListUsers.csv", ios::ate);
    if(!ifs.is_open() || !ofs.is_open()){
        cout << "Failed when opening file\n";
        return;
    }
    bool flag = true;
    if(ifs.tellg() == 0){
        flag = false;
    }
    if(!flag){
        ofs << u.username << "," << u.password;
        return;
    }
    ofs << "\n" << u.username << "," << u.password;
    ofs.close();
}
void readUser(hashTable& h){
    ifstream ifs;
    ifs.open("DataUsers/ListUsers.csv");
    if(!ifs.is_open()){
        cout << "Failed when opening file\n";
        return;
    }
    while(!ifs.eof()){
        string name{};
        string pass{};
        getline(ifs, name, ',');
        getline(ifs, pass);
        h.add(name, pass);
    }
    ifs.close();
}
user login(hashTable& h){
    user u{};
    bool flag = true;
    while(flag){
        menuLogin();
        int choose;
        cout << "Enter your choice: ";
        cin >> choose;
        switch(choose){
            case 1:{
                string username{};
                cout << "Enter your username and password\n";
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                string pass = hiddenPass();
                while(h.searchValue(username) == "" || h.searchValue(username) != pass){
                    clearScreen();
                    cout << "Your username/password is not correct. Please enter again !\n";
                    //or enter 'B' to return
                    cout << "Enter your username and password\n";
                    cout << "Username: ";
                    cin >> username;
                    cout << "Password: ";
                    pass = hiddenPass();
                }
                u.password = pass;
                u.username = username;
                cout << "Sign in is succesfull\n";
                Sleep(1000);
                clearScreen();
                return u;
            }
            case 2:{
                string username{};
                cout << "Enter username and password you want to sign up\n";
                cout << "Username and password only have digits, lower and upper letter. Username/Password must have 8->20 character\n";
                cout << "Username: ";
                cin >> username;
                while(!checkUsername(username)){
                    clearScreen();
                    cout << "Username is not correct. Please enter again!\n";
                    cout << "Username and password only have digits, lower and upper letter. Username/Password must have 8->20 character\n";
                    cout << "Username: ";
                    cin >> username;
                }
                while(h.searchValue(username) != ""){
                    clearScreen();
                    cout << "Username is valid. Please enter again!\n";
                    cout << "Username and password only have digits, lower and upper letter. Username/Password must have 8->20 character\n";
                    cout << "Username: ";
                    cin >> username;
                }
                cout << "Password: ";
                string pass = hiddenPass();
                while(!checkPassWord(pass)){
                    cout << "Password is not correct. Please enter again!\n";
                    cout << "Username and password only have digits, lower and upper letter. Username/Password must have 8->20 character\n";
                    cout << "Password: ";
                    pass = hiddenPass();
                }
                cout << "Enter your password again: ";
                string passAgain = hiddenPass();
                while(pass!=passAgain){
                    cout << "You have already have a wrong password. Please enter again!\n";
                    cout << "Enter your password again: ";
                    passAgain = hiddenPass();
                }
                h.add(username, pass);
                u.password = pass;
                u.username = username;
                writeUsersToFile(u);
                fs::create_directory("DataUsers/" + username);
                cout << "Sign up is succesfull\n";
                Sleep(1000);
                clearScreen();
                break;
            }
            default:{
                cout << "You have already entered a wrong choice! Please enter again!!!\n";
                break;
            }
        }
    }
}
// Ham lon
bool isEmpty(string filename){
    ifstream ifs;
    ifs.open(filename);
    int size = ifs.tellg();
    ifs.close();
    if(size == 0){
        return true;
    }
    return false;
}

void newGame(user u, bool isCon)
{
    int rows = 0;
    int cols = 0;
    int numsOfBomb = 0;
    string levelToUseInFile{};
    bool isFirst = true;
    vector<vector<char>> boardToDisplay{};
    vector<vector<char>> visit{};
    vector<vector<char>> board{};
    string filename = "DataUsers/" + u.username + "/boardToDisplay.txt";
    string filename1 = "DataUsers/" + u.username + "/board.txt";
    string filename2 = "DataUsers/" + u.username + "/visit.txt";
    if(!isCon){
        bool isCancel = false;
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
                clearScreen();
                cout << "Do you really want to exit this game?(Y/N): \n";
                string isExit{};
                cin >> isExit;
                clearScreen();
                if (isExit == "N")
                {
                    break;
                }
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
            return;
        }
        boardToDisplay = vector<vector<char>>(rows, vector<char>(cols, 'E'));
        visit = vector<vector<char>>(rows, vector<char>(cols, '0'));
        board = vector<vector<char>>(rows, vector<char>(cols, 'E'));
    }else{
        isFirst = false;
        extractBoardFromFile(boardToDisplay, levelToUseInFile, filename, rows, cols, numsOfBomb);
        extractBoardFromFile(board, levelToUseInFile, filename1, rows, cols, numsOfBomb);
        extractBoardFromFile(visit, levelToUseInFile, filename2, rows, cols, numsOfBomb);
    }
    auto start = chrono::steady_clock::now();;
    bool isStartTime = false;
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
            if (!isFirst)
            {
                clearScreen();
                cout << "Do you really want to exit this game?(Y/N): \n";
                string isExit{};
                cin >> isExit;
                if (isExit == "N")
                {
                    clearScreen();
                    break;
                }
                else
                {
                    clearScreen();
                    isFirst = true;
                    cout << "Do you want to save this game?(Y/N): \n";
                    string s{};
                    cin >> s;
                    if(s == "N"){
                        fs::remove(filename);
                        fs::remove(filename1);
                        fs::remove(filename2);
                    }else{
                        cout << "Save succesfully!\n";
                        Sleep(1000);
                    }
                }
            }
            numsOfBomb = INT_MAX;
            break;
        }
        case 1:
        {
            if (!isFirst)
            {
                clearScreen();
                cout << "Do you really want to restart this game?(Y/N): \n";
                string isRestart{};
                cin >> isRestart;
                if (isRestart == "N")
                {
                    break;
                }
                else
                {
                    isFirst = true;
                }
            }
            fs::remove(filename);
            fs::remove(filename1);
            fs::remove(filename2);
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
            while (p.first < 0 || p.second < 0 || p.first >= rows || p.second >= cols || (boardToDisplay[p.first][p.second] > '0' && boardToDisplay[p.first][p.second] < '9') || boardToDisplay[p.first][p.second] == ' ')
            {
                cout << "You have already entered wrong choice. Please enter again!\n";
                cout << "Enter your square you want to set flag: ";
                cin >> p.first >> p.second;
            }
            click.push_back(p.first);
            click.push_back(p.second);
            clearScreen();
            if (boardToDisplay[click[0]][click[1]] == 'F')
            {
                unSetFlag(boardToDisplay, click);
                if(!isStartTime){
                    start = chrono::steady_clock::now();
                    isStartTime = true;
                }
            }
            else
            {
                setFlag(boardToDisplay, click);
                if(!isStartTime){
                    start = chrono::steady_clock::now();
                    isStartTime = true;
                }
            }
            string filename = "DataUsers/" + u.username + "/boardToDisplay.txt";
            writeBoardToFile(boardToDisplay, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename);
            if (count('E', boardToDisplay) + count('F', boardToDisplay) == numsOfBomb)
            {
                fs::remove(filename);
                fs::remove(filename1);
                fs::remove(filename2);
                auto end = chrono::steady_clock::now();
                auto t = chrono::duration_cast<chrono::seconds>(end - start).count();
                int h = t / 3600;
                int m = (t % 3600) / 60;
                int s = (t % 60);
                //string fileName = "";
                //writeTimeToFile(h, m, s, fileName);
                clearScreen();
                printWhenEndGame(boardToDisplay, board, numsOfBomb);
                cout << endl
                     << "You are win!\n";
                cout << "Press any key to play game: ";
                cin.ignore();
                cin.get();
                newGame(u, false);
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
            while (p.first < 0 || p.second < 0 || p.first >= rows || p.second >= cols || boardToDisplay[p.first][p.second] == 'F' || (boardToDisplay[p.first][p.second] > '0' && boardToDisplay[p.first][p.second] < '9') || boardToDisplay[p.first][p.second] == ' ')
            {
                cout << "You have already entered wrong choice. Please enter again!\n";
                cout << "Enter your square you want to open: ";
                cin >> p.first >> p.second;
            }
            click.push_back(p.first);
            click.push_back(p.second);
            clearScreen();
            if (board[click[0]][click[1]] == 'M')
            {
                fs::remove(filename);
                fs::remove(filename1);
                fs::remove(filename2);
                board = updateBoard(board, click, visit);
                copyBoardToDisplay(boardToDisplay, board, visit);
                clearScreen();
                cout << "Bummm....\n";
                Sleep(50);
                clearScreen();
                printWhenEndGame(boardToDisplay, board, numsOfBomb);
                cout << "End Game!\n";
                cout << "Press any key to play game: ";
                cin.ignore();
                cin.get();
                clearScreen();
                newGame(u, false);
                numsOfBomb = INT_MAX;
                break;
            }
            if (isFirst)
            {
                board = createBoard(numsOfBomb, click, rows, cols);
                isFirst = false;
            }
            if(!isStartTime){
                start = chrono::steady_clock::now();
                isStartTime = true;
            }
            board = updateBoard(board, click, visit);
            copyBoardToDisplay(boardToDisplay, board, visit);
            writeBoardToFile(boardToDisplay, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename);
            writeBoardToFile(board, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename1);
            writeBoardToFile(visit, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename2);
            if (count('E', boardToDisplay) + count('F', boardToDisplay) == numsOfBomb)
            {
                fs::remove(filename);
                fs::remove(filename1);
                fs::remove(filename2);
                clearScreen();
                printWhenEndGame(boardToDisplay, board, numsOfBomb);
                cout << endl
                     << "You are win!\n";
                cout << "Press any key to play game: ";
                cin.ignore();
                cin.get();
                clearScreen();
                newGame(u, false);
                numsOfBomb = INT_MAX;
                break;
            }
            break;
        }
        default:{
            cout << "You have already entered wrong choice. Please enter again!\n";
            break;
        }
        }
    }

    return;
}
void option(){
    
}
void playGame(hashTable& h)
{
    while(1){
        user u = login(h);
        // Dat co de quan ly viec choi va thoat khoi game
        bool flag = true;
        bool isChoice = false;
        while (flag)
        {
            // tao menu lua chon cho user nhap vao
            ifstream ifs;
            ifs.open("DataUsers/" + u.username + "/board.txt");
            if(!ifs.is_open()){
                menuGameNotCon(u);
                isChoice = false;
            }else{
                menuGame(u);
                isChoice = true;
            }
            ifs.close();
            int choose;
            cout << "Enter your choice: ";
            cin >> choose;
            clearScreen();
            switch (choose)
            {
            // exit
            case 0:
            {
                clearScreen();
                cout << "Do you really want to sign out?(Y/N): \n";
                string isExit{};
                cin >> isExit;
                clearScreen();
                if (isExit == "N")
                {
                    break;
                }
                flag = false;
                break;
            }
            // New Game
            case 1:
            {
                newGame(u, false);
                break;
            }
            case 2:
            {
            }
            case 3:
            {
            }
            case 4:{
                if(!isChoice){
                    cout << "You have already entered wrong choice. Please enter again!\n";
                    break;
                }
                newGame(u, true);
                break;
            }
            default:{
                cout << "You have already entered wrong choice. Please enter again!\n";
                break;
            }
            }
        }
    }
    
}