#include "function.h"
int main()
{
    hashTable h{};
    h.init(1000);
    h.add("qthang", "1234567");
    user u = login(h);
    playGame();
    return 0;
}