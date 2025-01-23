#include "function.h"
int hashSize = 10000;
int main()
{
    listNode list = initList();
    
    hashTable h{};
    h.init(hashSize);
    readUser(h);
    playGame(h, list);
    return 0;
}