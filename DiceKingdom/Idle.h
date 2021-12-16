#pragma once
#include "Place.h"

class Idle : public Place
{
	public:
	Idle() : Place("Idle", 0) {}
	unsigned int create_resources();
};