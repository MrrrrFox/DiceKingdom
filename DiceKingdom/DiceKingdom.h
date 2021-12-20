#pragma once
#include "Place.h"
#include "Lumber.h"
#include "PaintRig.h"
#include "Idle.h"

struct Material
{
	Material(const std::string& _name, int _quantity = 0) : name(_name), quantity(_quantity) {}

	std::string name;
	int quantity;
};

struct Resources
{
	Resources()
	{
		available_materials_vector = {&wood, &paint};
		map_of_materials["Wood"] = &wood;
		map_of_materials["Paint"] = &paint;
		map_of_materials["Stone"] = &some_super_advanced_material;
	}

	Material wood{"Wood"};
	Material paint{"Paint"};
	Material some_super_advanced_material{"Stone"};

	std::vector<Material*> available_materials_vector;

	std::map<std::string, Material*, std::less<>> map_of_materials;
};

struct PlaceWithLimitedInformation
{
	PlaceWithLimitedInformation(int _x = 0, int _y = 0) : position(std::make_pair(_x, _y)) {}

	unsigned int last_roll = 0;
	std::pair<int, int> position;
};

class DiceKingdom
{
	Lumber lumber;
	PaintRig rig;
	Idle idle;

	Resources resources;
	std::map<std::string, PlaceWithLimitedInformation, std::less<>> map_of_places_with_limited_information;

	std::map<std::string, Place*, std::less<>> map_of_buildings;

	public:
	DiceKingdom();

	void add_materials(std::map<std::string, unsigned int, std::less<>> m);
	bool is_empty(const std::string& place);
	int count_dices(const std::string& place);
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> return_dice_array(const std::string& place);
	std::set<DiceWithoutHP, DiceCompareWithoutHP> return_dice_array_combined_with_idle(const std::string& place);

	void create_resources();

	void add_dice(const std::string& place, const Dice d, unsigned int n = 1);
	Dice find_most_damaged_dice(DiceWithoutHP dice, const std::string& place);
	Dice find_least_damaged_dice(DiceWithoutHP dice, const std::string& place);
	void remove_dice(const std::string &place, const Dice d, unsigned int n = 1);

	std::vector<Material*> get_resources() const
	{
		return resources.available_materials_vector;
	}
	std::map<std::string, PlaceWithLimitedInformation, std::less<>> get_map_of_places_with_limited_information() const
	{
		return map_of_places_with_limited_information;
	}
};