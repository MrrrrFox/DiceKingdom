#include "pch.h"
#include "DiceKingdom.h"
#include "GameManager.h"

int main()
{
	constexpr bool backend = false;

	try
	{
		if(!backend)
		{
			constexpr unsigned int WIDTH = 600;
			constexpr unsigned int HEIGHT = 400;
			const sf::ContextSettings context(24, 0, 0, 4, 5);
			sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Dice Kingdom", 7U, context);
			GameManager Game(&window, WIDTH, HEIGHT);

			Game.Run();

			Game.CloseGame();
		}
		else
		{
			std::map<std::string, unsigned int, std::less<>> initialize;
			initialize["Paint"] = 10;
			DiceKingdom dk{initialize};
			dk.add_dice("Lumber Camp", Dice(6), 400);
			dk.add_dice("Lumber Camp", Dice(8), 2);
			dk.add_dice("Paint Rig", Dice(4));
			dk.add_dice("Idle", Dice(10));
			auto m = dk.return_dice_array_combined_with_idle("Lumber Camp");
			for(auto it : m)
			{
				std::cout << it.faces << '\n';
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "\nError: " << e.what();
	}
	return 0;
}