#pragma once
#include "enums.h"
#include "Place.h"
#include "Spherical.h"
#include "Terrain.h"
#include "DiceKingdom.h"

class SceneLayout
{
	using fun = void (SceneLayout::*)(int, int) const;

	unsigned int WIDTH = 600;
	unsigned int HEIGHT = 400;
	float width_to_height = static_cast<float> (WIDTH) / static_cast<float> (HEIGHT);
	bool perspective_projection = false;
	std::pair<float, float> map_origin{0.0f, 0.0f};

	sf::Texture menuTexture;
	Spherical menu_camera = Spherical(3.0f, 3.14f / 2, 0.0f);

	sf::Texture unknownTerrainTexture;
	std::pair<int, int> player_position;
	std::pair<float, float> hex_size{sqrt(3.0f), 2.0f};

	std::map<std::string, fun, std::less<>> drawing_places;
	Spherical playing_camera = Spherical(3.0f, 1.2f, 0.7f);

	public:
	explicit SceneLayout(float hex_size);
	void initScene(std::pair<unsigned int, unsigned int> window_sizes);
	void reshapeScreen(sf::Vector2u size) const noexcept;

	bool get_perspective_projection() const noexcept;
	Spherical* get_playing_camera() noexcept;
	void set_perspective_projection(bool _perspective_projection) noexcept;
	void set_width_height(unsigned int width, unsigned int height) noexcept;
	void set_map_origin(int _x_map_size, int _z_map_size) noexcept;
	void set_hex_size(float _size) noexcept;

	void DrawMenu() const;

	std::pair<float, float> CalculateHexPosition(int row, int column) const noexcept;
	void DrawColorHex(std::array<GLfloat, 3> color, int row, int column) const noexcept;

	void DrawWorldMap(const std::vector<Terrain>& world_map) const;
	void DrawTerrain(Terrain terrain) const;
	void DrawUknown(int row, int column, std::pair<int, int> min_max_x, std::pair<int, int> min_max_z) const;
	void DrawCastle(int row, int column) const;

	void DrawKingdom(std::map<std::string, PlaceWithLimitedInformation, std::less<>> places, float time_to_proc);
	void DrawIdle(int row, int column) const noexcept;
	void DrawLumber(int row, int column) const;
	void DrawRig(int row, int column) const;
};