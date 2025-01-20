#include "function.h"
int hashSize = 10000;
int main()
{
    hashTable h{};
    h.init(hashSize);
    readUser(h);
    playGame(h);
    return 0;
}