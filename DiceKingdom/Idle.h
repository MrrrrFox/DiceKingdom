#pragma once
#include "Place.h"

class Idle : public Place
{
	public:
	explicit Idle(const std::string& name) : Place(name, 0) {}
	unsigned int create_resources() final;
};