#pragma once
#include <vector>
#include "enums.h"
#include "Place.h"

class GraphicsManager
{
	sf::Texture menuTexture;

public:
	GraphicsManager();
	void DrawMenu();
	void DrawWorldMap(std::vector<std::vector<WorldTerrain>> world_map);
	void DrawTerrain(WorldTerrain terrain, int x_pos, int y_pos);
	void DrawKingdom(std::vector<Place *> places, GameView game_view);
};