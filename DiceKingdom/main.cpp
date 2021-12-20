#include "pch.h"
#include "DiceKingdom.h"
#include "GameManager.h"

int main()
{
	bool backend = false;

	try
	{
		if(!backend)
		{
			unsigned int WIDTH = 600;
			unsigned int HEIGHT = 400;
			sf::ContextSettings context(24, 0, 0, 4, 5);
			sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Dice Kingdom", 7U, context);
			GameManager Game(&window, WIDTH, HEIGHT);

			Game.Run();

			Game.CloseGame();
		}
		else
		{
			DiceKingdom dk;
			std::map<std::string, unsigned int, std::less<>> initialize;
			initialize["Paint"] = 10;
			dk.add_materials(initialize);
			dk.add_dice("Lumber Camp", Dice(6), 400);
			dk.add_dice("Lumber Camp", Dice(8), 2);
			dk.add_dice("Paint Rig", Dice(4));
			dk.add_dice("Idle", Dice(10));
			auto m = dk.return_dice_array_combined_with_idle("Lumber Camp");
			for(auto it = m.begin(); it != m.end(); it++)
			{
				std::cout << it->faces << '\n';
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "\nError: " << e.what();
	}
	return 0;
}