#include "pch.h"
#include "DiceKingdom.h"
#include "GameManager.h"

int main()
{
    bool backend = false;

    try
    {
        if (!backend)
        {
            int WIDTH = 600, HEIGHT = 400;
            sf::ContextSettings context(24, 0, 0, 4, 5);
            sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Dice Kingdom", 7U, context);
            GameManager Game(&window, WIDTH, HEIGHT);

            Game.Run();

            Game.CloseGame();
        }
        else
        {
            std::srand(std::time(NULL));
            DiceKingdom dk;
            std::map<std::string, unsigned int> initialize;
            initialize["Paint"] = 10;
            dk.add_materials(initialize);
            dk.add_dice("Lumber", Dice(6), 4);
            dk.add_dice("Lumber", Dice(8), 2);
            auto m = dk.map_of_buildings["Lumber"]->return_dice_array();
            for (auto it = m.begin(); it != m.end(); it++)
            {
                std::cout << it->first.faces << "    " << it->second << '\n';
            }
            int count = 1;
            while (!dk.map_of_buildings["Lumber"]->is_empty())
            {
                std::cout << count << '\t' << dk.map_of_buildings["Lumber"]->count_dices() << '\t' << dk.map_of_buildings["Lumber"]->roll() << "\n";
                count++;
            }
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "\nError: " << e.what();
    }
    return 0;
}