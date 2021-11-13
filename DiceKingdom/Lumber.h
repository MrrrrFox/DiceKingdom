#pragma once
#include "Places.h"

class Lumber : public Place
{
	Lumber() : Place("Lumber Camp") {}
	public:
	int create_resources();
};