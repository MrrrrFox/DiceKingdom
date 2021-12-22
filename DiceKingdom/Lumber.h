#pragma once
#include "Place.h"

class Lumber : public Place
{
	public:
	explicit Lumber(const std::string& name) : Place(name) {}
	unsigned int create_resources() final;
};