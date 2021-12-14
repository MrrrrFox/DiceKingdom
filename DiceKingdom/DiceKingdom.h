#pragma once
#include "Place.h"
#include "Lumber.h"
#include "PaintRig.h"

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
		available_materials_vector = {wood, paint};
		map_of_materials["Wood"] = &wood;
		map_of_materials["Paint"] = &paint;
		map_of_materials["Stone"] = &some_super_advanced_material;
	}

	Material wood;
	Material paint;
	Material some_super_advanced_material;

	std::vector<Material> available_materials_vector;

	std::map<std::string, Material*> map_of_materials;
};

struct PlaceWithLimitedInformation
{
	PlaceWithLimitedInformation(std::string _name, unsigned int _last_roll = 0) : name(_name), last_roll(_last_roll) {}

	std::string name;
	unsigned int last_roll;
};

class DiceKingdom
{
	Lumber lumber;
	PaintRig rig;

	Resources resources;
	std::vector<PlaceWithLimitedInformation> vector_of_places_with_limited_information;


	public:
	DiceKingdom();

	std::map<std::string, Place*> map_of_buildings;//move to private after debugging

	void add_materials(std::map<std::string, unsigned int> m);
	void add_dice(std::string place, Dice d, int n = 1);

	std::vector<Material> get_resources()
	{
		return resources.available_materials_vector;
	}
	std::vector<PlaceWithLimitedInformation> get_vector_of_places_with_limited_information()
	{
		return vector_of_places_with_limited_information;
	}
};