#pragma once
#include "Place.h"

class PaintRig : public Place
{
	public:
	PaintRig() : Place("Paint Rig", 0.1f) {}
	unsigned int create_resources() final;
};