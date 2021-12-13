#pragma once
#include "Place.h"

class Lumber : public Place
{
	public:
	Lumber() : Place("Lumber Camp") {}
	int create_resources();
};