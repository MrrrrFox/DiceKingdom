#pragma once
#include "enums.h"

struct Terrain
{
	TerrainType terrainType;
	bool visible;

	Terrain(TerrainType _terrainType, bool _visible=false) : terrainType(_terrainType), visible(_visible) {}
};