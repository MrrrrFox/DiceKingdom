#include "pch.h"
#include "DiceKingdom.h"

DiceKingdom::DiceKingdom()
{
	int* _paint = &resources.paint.quantity;
	lumber.set_paint(_paint);
	rig.set_paint(_paint);

	vector_of_places_with_limited_information = {
		PlaceWithLimitedInformation(lumber.get_name()),
		PlaceWithLimitedInformation(rig.get_name())
	};

	map_of_buildings["Lumber"] = &lumber;
	map_of_buildings["PaintRig"] = &rig;
}

void DiceKingdom::add_materials(std::map<std::string, unsigned int> m)
{
	for(auto it = m.begin(); it != m.end(); it++)
	{
		resources.map_of_materials[it->first]->quantity += it->second;
	}
}

void DiceKingdom::add_dice(std::string place, Dice d, unsigned int n)
{
	map_of_buildings[place]->add(d, n);
}

bool DiceKingdom::is_empty(std::string place)
{
	return map_of_buildings[place]->is_empty();
}

int DiceKingdom::count_dices(std::string place)
{
	return map_of_buildings[place]->count_dices();
}

std::map<DiceWithoutHP, int, DiceCompareWithoutHP> DiceKingdom::return_dice_array(std::string place)
{
	return map_of_buildings[place]->return_dice_array();
}

void DiceKingdom::create_resources()
{
	lumber.create_resources();
	rig.create_resources();
}
