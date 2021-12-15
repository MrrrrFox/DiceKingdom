#include "pch.h"
#include "DiceKingdom.h"
#include "GameManager.h"

int main()
{
	try
	{/*
		int WIDTH = 600, HEIGHT = 400;
		sf::ContextSettings context(24, 0, 0, 4, 5);
		sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Dice Kingdom", 7U, context);
		GameManager Game(&window, WIDTH, HEIGHT);

		Game.Run();

		Game.CloseGame();
		*/

		std::srand(static_cast<unsigned int>(std::time(NULL)));
		DiceKingdom dk;
		std::map<std::string, unsigned int> initialize;
		initialize["Paint"] = 10;
		dk.add_materials(initialize);
		dk.add_dice("Lumber Camp", Dice(6), 400);
		dk.add_dice("Lumber Camp", Dice(8), 2);
		dk.add_dice("Paint Rig", Dice(4));
		auto m = dk.return_dice_array("Lumber Camp");
		for(auto it = m.begin(); it != m.end(); it++)
		{
			std::cout << it->first.faces << "    " << it->second << '\n';
		}
		int count = 1;
		while(!dk.is_empty("Lumber Camp"))
		{
			std::cout << count << '\t' << dk.count_dices("Lumber Camp") << '\t';
			dk.create_resources();
			std::cout << dk.get_resources()[0]->quantity << '\t' << dk.get_resources()[1]->quantity << "\n";
			count++;
		}

	}
	catch(std::exception& e)
	{
		std::cerr << "\nError: " << e.what();
	}
	return 0;
}