#include "pch.h"
#include "DiceKingdom.h"

DiceKingdom::DiceKingdom()
{
	int* _paint = &resources.paint.quantity;
	lumber.set_paint(_paint);
	rig.set_paint(_paint);
	idle.set_paint(_paint);

	map_of_places_with_limited_information["Lumber Camp"] = PlaceWithLimitedInformation(0, -1);
	map_of_places_with_limited_information["Paint Rig"] = PlaceWithLimitedInformation(1, 0);
	map_of_places_with_limited_information["Idle"] = PlaceWithLimitedInformation(0, 0);

	map_of_buildings["Lumber Camp"] = &lumber;
	map_of_buildings["Paint Rig"] = &rig;
	map_of_buildings["Idle"] = &idle;
}

void DiceKingdom::add_materials(std::map<std::string, unsigned int> m)
{
	for(auto it = m.begin(); it != m.end(); it++)
	{
		resources.map_of_materials[it->first]->quantity += it->second;
	}
}

void DiceKingdom::add_dice(std::string place, const Dice d, unsigned int n)
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

std::set<DiceWithoutHP, DiceCompareWithoutHP> DiceKingdom::return_dice_array_combined_with_idle(std::string place)
{
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> map_place = return_dice_array(place);
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> map_idle = return_dice_array("Idle");
	std::set<DiceWithoutHP, DiceCompareWithoutHP> ret;
	for(auto it = map_place.begin(); it != map_place.end(); it++)
	{
		ret.insert(it->first);
	}
	for(auto it = map_idle.begin(); it != map_idle.end(); it++)
	{
		ret.insert(it->first);
	}
	return ret;
}

void DiceKingdom::create_resources()
{
	unsigned int temp;

	temp = rig.create_resources(); // create paint before doing anything else
	resources.paint.quantity += temp;
	map_of_places_with_limited_information[rig.get_name()].last_roll = temp;

	// create other resources in some arbitrary order
	temp = lumber.create_resources();
	resources.wood.quantity += temp;
	map_of_places_with_limited_information[lumber.get_name()].last_roll = temp;

	idle.create_resources(); // fixing dices in idle should have low priority

	// creating dices uses significant amount of paint so it should be done last
}

const Dice DiceKingdom::find_most_damaged_dice(DiceWithoutHP dice, std::string place)
{
	Dice d(dice, max_dmg);
	std::map<Dice, int, DiceCompare> m = (*map_of_buildings[place]->get_map());
	auto it = m.find(d);
	while(it == m.end())
	{
		if(d.damage == 0)
			throw std::runtime_error("Invalid value");
		d.damage--;
		it = m.find(d);
	}
	return d;
}

const Dice DiceKingdom::find_least_damaged_dice(DiceWithoutHP dice, std::string place)
{
	Dice d(dice, 0);
	std::map<Dice, int, DiceCompare> m = (*map_of_buildings[place]->get_map());
	auto it = m.find(d);
	while(it == m.end())
	{
		if(d.damage > max_dmg)
			throw std::runtime_error("Invalid value");
		d.damage++;
		it = m.find(d);
	}
	return d;
}

void DiceKingdom::remove_dice(std::string place, const Dice d, unsigned int n)
{
	map_of_buildings[place]->remove(d, n);
}
