#pragma once
#include "Place.h"

class PaintRig : public Place
{
	public:
	PaintRig() : Place("Paint Rig") {}
	unsigned int create_resources();
};