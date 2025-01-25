#include "function.h"

// CONSTANT
// Easy Level
int rowsEasyLevel = 8;
int colsEasyLevel = 8;
int numsOfBombInEasyLevel = 10;
// Medium Level
int rowsMediumLevel = 16;
int colsMediumLevel = 16;
int numsOfBombInMediumLevel = 40;
// Hard Level
int rowsHardLevel = 16;
int colsHardLevel = 30;
int numsOfBombInHardLevel = 99;
// Save The Game Before
vector<vector<char>> tmpBoard{};
vector<vector<bool>> tmpVisit{};
vector<vector<char>> tmpBoardToDisplay{};
bool tmpIsFirst = false;
// The path to save the game after it was abruptly close
string pathToSave = "DataUser/username/gameSave";
bool stopThread = false;

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
    cout << setw(30) << u.username << endl;
    cout << "0. Back\n";
    cout << "1. My Highscores\n";
    cout << "2. About\n";
    cout << "3. Help\n";
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
bool isInput(string input){
    if(input[0] < '0' || input[0] > '9' || input.size() != 1){
        return false;
    }
    return true;
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
void writeTimeToFile(string fileName, int x)
{
    auto start = chrono::steady_clock::now();
    while(!stopThread){
        auto cur = chrono::steady_clock::now();
        auto ago = x;
        auto timePlay = chrono::duration_cast<chrono::seconds>(cur - start).count() + x;
        int hours = timePlay/3600;
        int minutes = (timePlay%3600)/60;
        int seconds = timePlay%60;
        ofstream ofs;
        ofs.open(fileName);
        ofs << hours << ":" << minutes << ":" << seconds;
        ofs.close();
    }
}
listNode initList(){
    listNode list{};
    list.head = nullptr;
    return list;
}
node* createNode(string no, string username, string time){
    node* n = new node;
    n->no = no;
    n->username = username;
    n->time = time;
    n->next = nullptr;
    return n;
}
int convertTimeToInt(node* n){
    if(n == nullptr){
        return 0;
    }
    int j = 2;
    int res = 0;
    string tmp{};
    for(int i = 0; i < n->time.size(); i++){
        if(n->time[i] == ':'){
            int t = stoi(tmp);
            res += t*pow(60,j--);
            tmp = {};
        }else{
            tmp += n->time[i];
        }
    }
    return res;
}
node* findToAdd(node* n, listNode list){
    int t = convertTimeToInt(n);
    if(list.head == nullptr || t <= convertTimeToInt(list.head)){
        return nullptr;
    }
    node* tmp = list.head;
    if(tmp->next == nullptr){
        return tmp;
    }
    while(tmp->next->next != nullptr){
        if(convertTimeToInt(tmp) > t && convertTimeToInt(tmp->next) <= t){
            return tmp;
        }
        tmp = tmp->next;
    }
    if(tmp->next->next == nullptr){
        if(convertTimeToInt(tmp) > t && convertTimeToInt(tmp->next) <= t){
            return tmp;
        }else{
            return tmp->next;
        }
    }
    return nullptr;
}
void removeNode(listNode& list){
    if(list.head == nullptr){
        return;
    }
    node* tmp = list.head;
    node* pre = nullptr;
    while(tmp->next == nullptr){
        pre = tmp;
        tmp = tmp->next;
    }
    pre->next = nullptr;
    tmp = nullptr;
    delete tmp;
}
void addNode(node* n, listNode& list){
    if(list.head == nullptr){
        list.head = n;
        return;
    }
    node* tmp = list.head;
    while(tmp->next != nullptr){
        tmp = tmp->next;
    }
    tmp->next = n;
}
void writeHighscoreToFile(string filename, listNode list){
    ofstream ofs(filename);
    if(!ofs.is_open()){
        return;
    }
    ofs << "No,Username,Time";
    if(list.head == nullptr){
        ofs << "No,Username,Time";
        return;
    }
    node* tmp = list.head;
    int i = 1;
    while(tmp != nullptr){
        int j = i;
        while(convertTimeToInt(tmp) == convertTimeToInt(tmp->next)){
            ofs << "\n" << to_string(j) << "," << tmp->username << "," << tmp->time;
            i++;
            tmp = tmp->next;
        }
        ofs << "\n" << to_string(i) << "," << tmp->username << "," << tmp->time;
        tmp = tmp->next;
    }
    ofs.close();
}
void getListHighscores(string filename, listNode& list){
    ifstream ifs;
    ifs.open(filename);
    if(!ifs.is_open()){
        return ;
    }
    string tmp{};
    getline(ifs, tmp);
    list = {};
    list.head = nullptr;
    while(!ifs.eof()){
        string tmp1{};
        string tmp2{};
        string tmp3{};
        getline(ifs, tmp1, ',');
        getline(ifs, tmp2, ',');
        getline(ifs, tmp3);
        node* n = createNode(tmp1, tmp2, tmp3);
        addNode(n, list);
    }
}
void addSort(node* n, listNode& list, string level){
    node* add = findToAdd(n, list);
    string filename = "DataActivity/Highscores" + level + ".csv";
    if(add == nullptr){
        node* tmp = list.head;
        n->next = tmp;
        list.head = n;
        list.size++;
        if(list.size > 100){
            removeNode(list);
        }
        writeHighscoreToFile(filename, list);
        getListHighscores(filename, list);
        return;
    }
    if(add->next == nullptr && list.size != 100){
        add->next = n;
        list.size++;
        if(list.size > 100){
            removeNode(list);
        }
        writeHighscoreToFile(filename, list);
        getListHighscores(filename, list);
        return;
    }
    node* tmp = add->next;
    add->next = n;
    n->next = tmp;
    list.size++;
    if(list.size > 100){
        removeNode(list);
    }
    writeHighscoreToFile(filename, list);
    getListHighscores(filename, list);
    return;
}
bool isEmpty(string filename){
    ifstream ifs;
    ifs.open(filename, ios::ate);
    int size = ifs.tellg();
    ifs.close();
    if(size == 0){
        return true;
    }
    return false;
}
int getSeconds(string fileName){
    ifstream ifs(fileName);
    if(!ifs.is_open() || isEmpty(fileName)){
        return 0;
    }
    string h{};
    string m{};
    string s{};
    getline(ifs,h,':');
    getline(ifs,m,':');
    getline(ifs,s);
    int res = stoi(h)*3600 + stoi(m)*60 + stoi(s);
    return res;
}
void updateRecord(string filename, string time){
    ofstream ofs(filename);
    ofs << time ;
    ofs.close();
    return;
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
        clearScreen();
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

bool isExist(string filename){
    ifstream ifs;
    ifs.open(filename);
    if(ifs.is_open()){
        ifs.close();
        return true;
    }
    ifs.close();
    return false;
}
void print(string filename){
    string tmp{};
    ifstream ifs(filename);
    if(!ifs.is_open()){
        return;
    }
    while(!ifs.eof()){
        string tmp1{};
        getline(ifs,tmp1);
        tmp += tmp1;
        tmp += "\n";
    }
    for(int i = 0; i < tmp.size(); i++){
        cout << tmp[i];
        //<< flush
        this_thread::sleep_for(chrono::milliseconds(12));
    }
    cout << endl;
}
string get(string filename){
    string res{};
    ifstream ifs(filename);
    if(!ifs.is_open()){
        return{};
    }
    getline(ifs, res);
    return res;
}
void newGame(user u, bool isCon, listNode& list)
{
    int rows = 0;
    int cols = 0;
    int numsOfBomb = 0;
    string levelToUseInFile{};
    bool isFirst = true;
    stopThread = false;
    vector<vector<char>> boardToDisplay{};
    vector<vector<char>> visit{};
    vector<vector<char>> board{};
    thread t{};
    string filename = "DataUsers/" + u.username + "/boardToDisplay.txt";
    string filename1 = "DataUsers/" + u.username + "/board.txt";
    string filename2 = "DataUsers/" + u.username + "/visit.txt";
    string filename3 = "DataUsers/" + u.username + "/time.txt";
    bool isStartTime = false;
    if(!isCon){
        bool isCancel = false;
        while (!isCancel)
        {
            menuLevel();
            int level;
            string input{};
            cout << "Enter your choice: ";
            cin >> input;
            while(!isInput(input)){
                clearScreen();
                cout << "You have already entered wrong choice. Please enter again!\n";
                cout << "Enter your choice: ";
                cin >> input;
            }
            level = stoi(input);
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
        int s = getSeconds(filename3);
        t = thread(writeTimeToFile,filename3, s);
        isStartTime = true;
        extractBoardFromFile(boardToDisplay, levelToUseInFile, filename, rows, cols, numsOfBomb);
        extractBoardFromFile(board, levelToUseInFile, filename1, rows, cols, numsOfBomb);
        extractBoardFromFile(visit, levelToUseInFile, filename2, rows, cols, numsOfBomb);
    }
    string filename4 = "DataUsers/" + u.username +"/Highscores" + levelToUseInFile + ".txt";
    if(!isExist(filename4)){
        ofstream ofs(filename4);
        ofs.close();
    }
    while (numsOfBomb != INT_MAX)
    {
        printWhenPlay(boardToDisplay, numsOfBomb);
        string input{};
        int choice;
        cout << "Enter your choice: ";
        cin >> input;
        while(!isInput(input)){
            clearScreen();
            cout << "You have already entered wrong choice. Please enter again!\n";
            cout << "Enter your choice: ";
            cin >> input;
        }
        choice = stoi(input);
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
                        if(isExist(filename3)){
                            fs::remove(filename3);
                        }
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
            if(isExist(filename3)){
                fs::remove(filename3);
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
                    int s = getSeconds(filename3);
                    t = thread(writeTimeToFile,filename3, s);
                    isStartTime = true;
                }
            }
            else
            {
                setFlag(boardToDisplay, click);
                if(!isStartTime){
                    int s = getSeconds(filename3);
                    t = thread(writeTimeToFile,filename3, s);
                    isStartTime = true;
                }
            }
            string filename = "DataUsers/" + u.username + "/boardToDisplay.txt";
            writeBoardToFile(boardToDisplay, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename);
            if (count('E', boardToDisplay) + count('F', boardToDisplay) == numsOfBomb)
            {
                stopThread = true;
                t.join();
                string ti = get(filename3);
                node* n = createNode("0", u.username, ti);
                addSort(n, list, levelToUseInFile);
                if((getSeconds(filename4) != 0 && getSeconds(filename4) > convertTimeToInt(n)) || getSeconds(filename4) == 0){
                    updateRecord(filename4, n->time);
                    clearScreen();
                    cout << "New Highscore !!!!\n";
                    cout << "Time: " << n->time << endl;
                    Sleep(1000);
                    cout << "Press any key to continue: ";
                    cin.ignore();
                    cin.get();
                }
                fs::remove(filename);
                fs::remove(filename1);
                fs::remove(filename2);
                fs::remove(filename3);
                clearScreen();
                printWhenEndGame(boardToDisplay, board, numsOfBomb);
                cout << endl
                     << "You are win!\n";
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                newGame(u, false, list);
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
                stopThread = true;
                t.join();
                fs::remove(filename);
                fs::remove(filename1);
                fs::remove(filename2);
                fs::remove(filename3);
                board = updateBoard(board, click, visit);
                copyBoardToDisplay(boardToDisplay, board, visit);
                clearScreen();
                cout << "Bummm....\n";
                Sleep(50);
                clearScreen();
                printWhenEndGame(boardToDisplay, board, numsOfBomb);
                cout << "Game Over!\n";
                cout << "Press any key to play game: ";
                cin.ignore();
                cin.get();
                clearScreen();
                newGame(u, false, list);
                numsOfBomb = INT_MAX;
                break;
            }
            if (isFirst)
            {
                board = createBoard(numsOfBomb, click, rows, cols);
                isFirst = false;
            }
            if(!isStartTime){
                int s = getSeconds(filename3);
                t = thread(writeTimeToFile,filename3, s);
                isStartTime = true;
            }
            board = updateBoard(board, click, visit);
            copyBoardToDisplay(boardToDisplay, board, visit);
            writeBoardToFile(boardToDisplay, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename);
            writeBoardToFile(board, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename1);
            writeBoardToFile(visit, levelToUseInFile, to_string(rows), to_string(cols), to_string(numsOfBomb), filename2);
            if (count('E', boardToDisplay) + count('F', boardToDisplay) == numsOfBomb)
            {
                stopThread = true;
                t.join();
                string ti = get(filename3);
                node* n = createNode("0", u.username, ti);
                addSort(n, list, levelToUseInFile);
                if((getSeconds(filename4) != 0 && getSeconds(filename4) > convertTimeToInt(n)) || getSeconds(filename4) == 0){
                    updateRecord(filename4, n->time);
                    clearScreen();
                    cout << "New Highscore !!!!\n";
                    cout << "Time: " << n->time << endl;
                    Sleep(1000);
                    cout << "Press any key to continue: ";
                    cin.ignore();
                    cin.get();
                }
                fs::remove(filename);
                fs::remove(filename1);
                fs::remove(filename2);
                fs::remove(filename3);
                clearScreen();
                printWhenEndGame(boardToDisplay, board, numsOfBomb);
                cout << endl
                     << "You are win!\n";
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                clearScreen();
                newGame(u, false, list);
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
void printHighscore(string filename){
    listNode list{};
    list.head = nullptr;
    getListHighscores(filename, list);
    cout << left << setw(4) << "No" << left << setw(25) << "username" << left << setw(12) << "Time" << endl;
    if(list.head == nullptr){
        return;
    }
    node* tmp = list.head;
    while(tmp != nullptr){
        cout << left << setw(4) << tmp->no << left << setw(25) << tmp->username << left << setw(12) << tmp->time << endl;
        tmp = tmp->next;
    }
}
void viewScoreBoard(){
    bool out = false;
    while(!out){
        clearScreen();
        menuLevel();
        string input{};
        int choice;
        cout << "Enter your choice: ";
        cin >> input;
        while(!isInput(input)){
            clearScreen();
            cout << "You have already entered wrong choice. Please enter again!\n";
            cout << "Enter your choice: ";
            cin >> input;
        }
        choice = stoi(input);
        switch(choice){
            case 0:{
                clearScreen();
                cout << "Do you really want to exit?(Y/N): \n";
                string isExit{};
                cin >> isExit;
                clearScreen();
                if (isExit == "N")
                {
                    break;
                }
                out = true;
                break;
            }
            case 1:{
                clearScreen();
                string filename = "DataActivity/HighscoresEasyLevel.csv";
                cout << "----------------HighscoresEasyLevel----------------\n";
                printHighscore(filename);
                Sleep(2000);
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                clearScreen();
                break;
            }
            case 2:{
                clearScreen();
                string filename = "DataActivity/HighscoresMediumLevel.csv";
                cout << "----------------HighscoresMediumLevel----------------\n";
                printHighscore(filename);
                Sleep(2000);
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                break;
            }
            case 3:{
                clearScreen();
                string filename = "DataActivity/HighscoresHardLevel.csv";
                cout << "----------------HighscoresHardLevel----------------\n";
                printHighscore(filename);
                Sleep(2000);
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                break;
            }
            default:
            {
                clearScreen();
                cout << "You have already entered a wrong choice! Please enter again!!!\n";
                Sleep(1000);
                break;
            }
        }
    }
}
void option(user u){
    bool out = false;
    while(!out){
        clearScreen();
        menuOption(u);
        string input{};
        int choice;
        cout << "Enter your choice: ";
        cin >> input;
        while(!isInput(input)){
            clearScreen();
            cout << "You have already entered wrong choice. Please enter again!\n";
            cout << "Enter your choice: ";
            cin >> input;
        }
        choice = stoi(input);
        switch(choice){
            case 0:{
                clearScreen();
                cout << "Do you really want to exit?(Y/N): \n";
                string isExit{};
                cin >> isExit;
                clearScreen();
                if (isExit == "N")
                {
                    break;
                }
                out = true;
                break;
            }
            case 1:{
                clearScreen();
                string filename = "DataUsers/" + u.username +"/HighscoresEasyLevel.txt";
                string filename1 = "DataUsers/" + u.username +"/HighscoresMediumLevel.txt";
                string filename2 = "DataUsers/" + u.username +"/HighscoresHardLevel.txt";
                if(isExist(filename) == true && isEmpty(filename) == false){
                    string tmp = get(filename);
                    cout << "Highscore Easy Level: " << tmp << endl;
                }
                if(isExist(filename1) == true && isEmpty(filename1) == false){
                    string tmp = get(filename1);
                    cout << "Highscore Medium Level: " << tmp << endl;
                }
                if(isExist(filename2) == true && isEmpty(filename2) == false){
                    string tmp = get(filename2);
                    cout << "Highscore Hard Level: " << tmp << endl;
                }
                Sleep(2000);
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                break;
            }
            case 2:{
                clearScreen();
                string filename = "DataActivity/About.txt";
                print(filename);
                Sleep(2000);
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                break;
            }
            case 3:{
                clearScreen();
                string filename = "DataActivity/Help.txt";
                print(filename);
                Sleep(2000);
                cout << "Press any key to continue: ";
                cin.ignore();
                cin.get();
                break;
            }
            default:
            {
                clearScreen();
                cout << "You have already entered a wrong choice! Please enter again!!!\n";
                Sleep(1000);
                break;
            }
        }
    }
}
void playGame(hashTable& h, listNode& list)
{
    while(1){
        clearScreen();
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
            string in{};
            int choose;
            cout << "Enter your choice: ";
            cin >> in;
            while(!isInput(in)){
                clearScreen();
                cout << "You have already entered wrong choice. Please enter again!\n";
                cout << "Enter your choice: ";
                cin >> in;
            }
            choose = stoi(in);
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
                newGame(u, false, list);
                break;
            }
            case 2:
            {
                viewScoreBoard();
                break;
            }
            case 3:
            {
                option(u);
            }
            case 4:{
                if(!isChoice){
                    cout << "You have already entered wrong choice. Please enter again!\n";
                    break;
                }
                newGame(u, true, list);
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