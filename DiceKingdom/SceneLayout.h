#pragma once
#include "enums.h"
#include "Place.h"
#include "Spherical.h"

class SceneLayout
{
	int WIDTH = 600, HEIGHT = 400;
	float width_to_height = (float)WIDTH/HEIGHT;
	bool perspective_projection = false;

	sf::Texture menuTexture;
	Spherical menu_camera = Spherical(3.0f, 3.14f/2, 0.0f);

public:
	SceneLayout();
	void initScene(int width, int height);
	void reshapeScreen(sf::Vector2u size);

	bool get_perspective_projection();
	void set_perspective_projection(bool _perspective_projection);
	void set_width_height(int width, int height);

	void DrawMenu();
	void DrawWorldMap(std::vector<std::vector<WorldTerrain>> world_map);
	void DrawTerrain(WorldTerrain terrain, int x_pos, int y_pos);
	void DrawKingdom();
	void RenderString(float position[], void* font, const unsigned char* string, float colors[]);
	//void drawText(const char* text, int text_length, int x_pos, int y_pos);
};