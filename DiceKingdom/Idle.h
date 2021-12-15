#pragma once
#include "Place.h"

class Idle : public Place
{
	public:
	Idle() : Place("Idle") {}
	unsigned int create_resources();
};