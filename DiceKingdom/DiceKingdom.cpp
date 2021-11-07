#include "pch.h"
#include "Places.h"

int main()
{
    std::srand(std::time(NULL));
    Place p;
    p.add(Dice(6), 2);
    std::cout<<p.roll();
    p.remove(Dice(6), 3);
    return 0;
}
