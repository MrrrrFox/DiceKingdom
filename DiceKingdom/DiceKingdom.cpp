#include "pch.h"
#include "Places.h"

int main()
{
    try
    {
        std::srand(std::time(NULL));
        Place p("Test");
        p.change_paint(10);
        p.add(Dice(6), 4);
        int count=1;
        while(!p.is_empty())
        {
            std::cout<<count<<'\t'<<p.roll()<<"\n";
            count++;
        }
    }
    catch(std::exception& e)
    {
        std::cerr<<"\nError: "<<e.what();
    }
    return 0;
}
