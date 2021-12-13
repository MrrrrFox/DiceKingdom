#include "pch.h"
#include "Lumber.h"
#include "GameManager.h"

int main()
{
    try
    {
        int WIDTH = 600, HEIGHT = 400;
        sf::ContextSettings context(24, 0, 0, 4, 5);
        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Dice Kingdom", 7U, context);
        GameManager Game(&window, WIDTH, HEIGHT);

        Game.Run();

        Game.CloseGame();

        /*
        std::srand(std::time(NULL));
        Lumber p;
        p.change_paint(10);
        p.add(Dice(6), 4);
        p.add(Dice(8), 2);
        auto m = p.return_dice_array();
        for(auto it = m.begin(); it != m.end(); it++)
        {
            std::cout << it->first.faces << "    " << it->second << '\n';
        }
        int count = 1;
        while(!p.is_empty())
        {
            std::cout << count << '\t' << p.count_dices() << '\t' << p.roll() << "\n";
            count++;
        }
        */

    }
    catch(std::exception& e)
    {
        std::cerr << "\nError: " << e.what();
    }
    return 0;
}