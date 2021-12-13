#pragma once
#include "Place.h"


class DiceKingdom
{
	std::vector<Place *> places;
	std::map<std::string, int> materials;

public:
	DiceKingdom();

	std::vector<Place *> get_places();

	void show_places();
	void clear();
};