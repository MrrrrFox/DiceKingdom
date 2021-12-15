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
	void initScene(std::pair<int,int> window_sizes, std::pair<int,int> DK_pos);
	void reshapeScreen(sf::Vector2u size);

	bool get_perspective_projection();
	void set_perspective_projection(bool _perspective_projection);
	void set_width_height(int width, int height);
	void set_map_origin(int _x_map_size, int _z_map_size);
	void set_hex_size(float _size);

	void DrawMenu();
	void DrawWorldMap(std::vector<std::vector<Terrain>> world_map);
	void DrawTerrain(TerrainType terrain, float x_posf, float z_posf);
	void DrawUknown(float x_posf, float z_posf, int x_map_size, int z_map_size);
	void DrawColorHex(GLfloat * color, float x_posf, float z_posf);
	void DrawKingdom();
	void DrawCastle(float x_posf, float z_posf);
	void RenderString(float position[], void* font, const unsigned char* string, float colors[]);
	//void drawText(const char* text, int text_length, int x_pos, int y_pos);
};