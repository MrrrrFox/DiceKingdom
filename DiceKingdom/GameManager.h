#pragma once
#include "enums.h"
#include "SceneLayout.h"
#include "ImGuiLayout.h"
#include "DiceKingdom.h"
#include "Terrain.h"


class GameManager
{
	GameView currentView = GameView::MENU;
	bool isRunning = true, isPlaying = false;
	unsigned int WIDTH = 600, HEIGHT = 400;

	SceneLayout sceneLayout;
	ImGuiLayout imGuiLayout;
	DiceKingdom DK;

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
	sf::Time deltaTime, procCountUpTime = sf::seconds(0.0f), procTime = sf::seconds(1.0f);

	public:
	GameManager(sf::RenderWindow* window, unsigned int WIDTH, unsigned int HEIGHT);
	void Run();
	void CloseGame();

	void DrawScene();
	void DrawImGui();
	void Proc();
};