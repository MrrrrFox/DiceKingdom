#pragma once
#include "enums.h"
#include "Place.h"
#include "Spherical.h"
#include "Terrain.h"

class SceneLayout
{
	int WIDTH = 600, HEIGHT = 400;
	float width_to_height = (float)WIDTH/HEIGHT;
	bool perspective_projection = false;
	std::pair<float, float> map_origin{ 0.0f, 0.0f };

	sf::Texture menuTexture;
	Spherical menu_camera = Spherical(3.0f, 3.14f / 2, 0.0f);

	sf::Texture unknownTerrainTexture;
	std::pair<int, int> player_position;
	std::pair<float, float> hex_size{ sqrt(3.0f), 2.0f };

	Spherical kingdom_camera = Spherical(3.0f, 1.2f, 0.7f);

public:
	Spherical map_camera = Spherical(3.0f, 1.2f, 0.7f); // TO DO: move to private
	SceneLayout();
	void initScene(std::pair<int,int> window_sizes);
	void reshapeScreen(sf::Vector2u size);

	bool get_perspective_projection();
	void set_perspective_projection(bool _perspective_projection);
	void set_width_height(int width, int height);
	void set_map_origin(int _x_map_size, int _z_map_size);
	void set_hex_size(float _size);

	void DrawMenu();
	void DrawWorldMap(std::vector<Terrain> world_map);
	void DrawTerrain(Terrain terrain);
	void DrawUknown(int row, int column, std::pair<int,int> min_max_x, std::pair<int, int> min_max_z);
	void DrawColorHex(GLfloat * color, int row, int column);
	void DrawKingdom();
	void DrawCastle(int row, int column);
	void RenderString(float position[], void* font, const unsigned char* string, float colors[]);
	//void drawText(const char* text, int text_length, int x_pos, int y_pos);
};