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
	int WIDTH = 600, HEIGHT = 400;

	SceneLayout sceneLayout;
	ImGuiLayout imGuiLayout;
	DiceKingdom DK;

	std::vector<std::vector<Terrain>> world_map = {
		{Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false),			Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false), Terrain(TerrainType::PLAIN, false), Terrain(TerrainType::PLAIN, false)},
		{Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::SEA, true),	Terrain(TerrainType::SEA, true),			Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false), Terrain(TerrainType::PLAIN, false)},
		{Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::SEA, true),	Terrain(TerrainType::DICE_KINGDOM, true),	Terrain(TerrainType::SEA, true),	Terrain(TerrainType::PLAIN, false)},
		{Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::SEA, true),	Terrain(TerrainType::SEA, true),			Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false), Terrain(TerrainType::PLAIN, false)},
		{Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false),			Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false)},
		{Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false)},
		{Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false),	Terrain(TerrainType::PLAIN, false)}
	};

	sf::RenderWindow* window;
	sf::Clock deltaClock;
	sf::Time deltaTime, procCountUpTime = sf::seconds(0.0f), procTime = sf::seconds(1.0f);

	public:
	GameManager(sf::RenderWindow* window, int WIDTH, int HEIGHT);
	void Run();
	void CloseGame();

	void DrawScene();
	void DrawImGui();
	void Proc();
	std::pair<int,int> get_DiceKingdom_position();
};