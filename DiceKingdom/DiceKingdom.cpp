#include "pch.h"
#include "DiceKingdom.h"
#include "Lumber.h"

DiceKingdom::DiceKingdom()
{
	materials["wood"] = 10;
	materials["paint"] = 20;

	lumber.add(Dice(4), 12);
	lumber.add(Dice(4,2), 3);
	lumber.add(Dice(6), 5);
	lumber.add(Dice(20), 1);
}

Place& DiceKingdom::get_place(KingdomPlace place)
{
	switch (place)
	{
	case KingdomPlace::LUMBER:
		return lumber;
	default:
		std::cerr << "No such place serviced: " << (int)place << std::endl;
	}
}

std::unordered_map<std::string, int> DiceKingdom::get_materials()
{
	return materials;
}

std::map<DiceWithoutHP, int, DiceCompareWithoutHP> DiceKingdom::get_dices_from(KingdomPlace place)
{
	switch (place)
	{
	case KingdomPlace::LUMBER:
		return lumber.return_dice_array();
	default:
		std::cerr << "No such place is serviced: " << (int)place << std::endl;
	}
	return std::map<DiceWithoutHP, int, DiceCompareWithoutHP>();
}