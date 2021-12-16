#pragma once
#include "enums.h"

struct Terrain
{
	TerrainType terrainType;
	bool visible;
	std::pair<int, int> position;

	Terrain(TerrainType _terrainType, bool _visible = false, std::pair<int, int> _position = {0,0}) : terrainType(_terrainType), visible(_visible), position(_position) {}
};