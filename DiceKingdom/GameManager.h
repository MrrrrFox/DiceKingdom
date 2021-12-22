#pragma once
#include "enums.h"
#include "SceneLayout.h"
#include "ImGuiLayout.h"
#include "DiceKingdom.h"
#include "Terrain.h"


class GameManager
{
	GameView currentView = GameView::MENU;
	bool isRunning = true;
	bool isPlaying = false;
	unsigned int WIDTH = 600;
	unsigned int HEIGHT = 400;

	SceneLayout sceneLayout{0.5f};
	ImGuiLayout imGuiLayout;
	DiceKingdom DK{std::map<std::string, unsigned int, std::less<>>()};

	std::vector<Terrain> world_map = {Terrain(TerrainType::DICE_KINGDOM, true, {0, 0}),
		Terrain(TerrainType::SEA, true, {0, 1}), Terrain(TerrainType::SEA, true, {0, -1}),
		Terrain(TerrainType::SEA, true, {1, 0}), Terrain(TerrainType::SEA, true, {1, -1}),
		Terrain(TerrainType::SEA, true, {-1, 0}), Terrain(TerrainType::SEA, true, {-1, -1}),
		Terrain(TerrainType::PLAIN, false, {0, 2}), Terrain(TerrainType::PLAIN, false, {0, 3}), Terrain(TerrainType::PLAIN, false, {0, -2}),
		Terrain(TerrainType::PLAIN, false, {-1, -2}), Terrain(TerrainType::PLAIN, false, {-1, -3}), Terrain(TerrainType::PLAIN, false, {-1, 1}),
		Terrain(TerrainType::PLAIN, false, {1, -2}), Terrain(TerrainType::PLAIN, false, {1, -3}), Terrain(TerrainType::PLAIN, false, {1, 1}),
		Terrain(TerrainType::PLAIN, false, {2, 0}), Terrain(TerrainType::PLAIN, false, {2, -2}), Terrain(TerrainType::PLAIN, false, {-2, 1}),
		Terrain(TerrainType::PLAIN, false, {-2, 2}), Terrain(TerrainType::PLAIN, false, {-2, -2}), Terrain(TerrainType::PLAIN, false, {-3, -3}),
	};

	sf::RenderWindow* window;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	sf::Time procCountUpTime = sf::seconds(0.0f);
	sf::Time procTime = sf::seconds(1.0f);

	public:
	GameManager(sf::RenderWindow* window, unsigned int WIDTH, unsigned int HEIGHT);
	void Run();
	void KeyboardEvent(sf::Event event);
	void MouseEvent(sf::Event event);
	void SwitchBuildings(sf::Event event) noexcept;
	void MapMovement(sf::Event event);
	void CloseGame() const;

	void DrawScene();
	void DrawImGui();
	void Proc();
};