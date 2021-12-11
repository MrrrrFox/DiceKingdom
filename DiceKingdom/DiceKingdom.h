#pragma once
#include "Place.h"


class DiceKingdom
{
	std::vector<Place *> places;

public:
	DiceKingdom();
	std::vector<Place *> get_places();
	void show_places();
	void clear();
};