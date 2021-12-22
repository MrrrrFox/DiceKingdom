#pragma once
#include "Place.h"

class PaintRig : public Place
{
	public:
	explicit PaintRig(const std::string& name) : Place(name, 0.1f) {}
	unsigned int create_resources() final;
};