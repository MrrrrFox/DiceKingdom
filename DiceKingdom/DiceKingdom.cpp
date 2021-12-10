#include "pch.h"
#include "DiceKingdom.h"
#include "Lumber.h"

DiceKingdom::DiceKingdom()
{
	Place * p1 = new Lumber;
	places.push_back(p1);
	Place* p2 = new Lumber;
	places.push_back(p2);
}

std::vector<Place *> DiceKingdom::get_places()
{
	return places;
}

void DiceKingdom::show_places()
{
	for(int i=0; i<places.size(); ++i)
		std::cout << places[i]->get_name() << " ";
	std::cout << std::endl;
}