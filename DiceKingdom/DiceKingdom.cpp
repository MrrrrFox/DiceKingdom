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

	lumber.add(Dice(4),12);
	lumber.add(Dice(4,2), 3);
	lumber.add(Dice(6),5);
	lumber.add(Dice(20));
}

void DiceKingdom::add_materials(std::map<std::string, unsigned int> m)
{
	for(auto it = m.begin(); it != m.end(); it++)
	{
		resources.map_of_materials[it->first]->quantity += it->second;
	}
}

void DiceKingdom::add_dice(std::string place, Dice d, int n)
{
	map_of_buildings[place]->add(d, n);
}
