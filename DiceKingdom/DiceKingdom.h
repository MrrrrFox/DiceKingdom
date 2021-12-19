#pragma once
#include "Place.h"
#include "Lumber.h"
#include "PaintRig.h"
#include "Idle.h"

struct Material
{
	Material(std::string _name, int _quantity = 0) : name(_name), quantity(_quantity) {}

	std::string name;
	int quantity;
};

struct Resources
{
	Resources() : wood("Wood"), paint("Paint"), some_super_advanced_material("Stone")
	{
		available_materials_vector = {&wood, &paint};
		map_of_materials["Wood"] = &wood;
		map_of_materials["Paint"] = &paint;
		map_of_materials["Stone"] = &some_super_advanced_material;
	}

	Material wood;
	Material paint;
	Material some_super_advanced_material;

	std::vector<Material*> available_materials_vector;

	std::map<std::string, Material*> map_of_materials;
};

struct PlaceWithLimitedInformation
{
	PlaceWithLimitedInformation(int _x = 0, int _y = 0) : last_roll(0), position(std::make_pair(_x, _y)) {}

	unsigned int last_roll;
	std::pair<int, int> position;
};

class DiceKingdom
{
	Lumber lumber;
	PaintRig rig;
	Idle idle;

	Resources resources;
	std::map<std::string, PlaceWithLimitedInformation> map_of_places_with_limited_information;

	std::map<std::string, Place*> map_of_buildings;

	public:
	DiceKingdom();

	void add_materials(std::map<std::string, unsigned int> m);
	bool is_empty(std::string place);
	int count_dices(std::string place);
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> return_dice_array(std::string place);
	std::set<DiceWithoutHP, DiceCompareWithoutHP> return_dice_array_combined_with_idle(std::string place);

	void create_resources();

	void add_dice(std::string place, const Dice d, unsigned int n = 1);
	const Dice find_most_damaged_dice(DiceWithoutHP dice, std::string place);
	const Dice find_least_damaged_dice(DiceWithoutHP dice, std::string place);
	void remove_dice(std::string place, const Dice d, unsigned int n = 1);

	std::vector<Material*> get_resources()
	{
		return resources.available_materials_vector;
	}
	std::map<std::string, PlaceWithLimitedInformation> get_map_of_places_with_limited_information()
	{
		return map_of_places_with_limited_information;
	}
};