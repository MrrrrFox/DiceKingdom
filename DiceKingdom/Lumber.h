#pragma once
#include "Place.h"

class Lumber : public Place
{
	public:
	Lumber() : Place("Lumber Camp") {}
	unsigned int create_resources() final;
};