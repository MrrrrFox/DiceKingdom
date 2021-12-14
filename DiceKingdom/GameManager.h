#pragma once
#include "enums.h"
#include "SceneLayout.h"
#include "DiceKingdom.h"
#include "ImGuiLayout.h"

class GameManager
{
	GameView currentView = GameView::MENU;
	bool isRunning = true, isPlaying = false;
	int WIDTH = 600, HEIGHT = 400;

	SceneLayout sceneLayout;
	ImGuiLayout imGuiLayout;
	DiceKingdom DK;

	std::vector<std::vector<TerrainType>> world_map = {
		{TerrainType::FOG_OF_WAR,	TerrainType::FOG_OF_WAR,	TerrainType::FOG_OF_WAR,		TerrainType::FOG_OF_WAR,	TerrainType::FOG_OF_WAR},
		{TerrainType::FOG_OF_WAR,	TerrainType::SEA,			TerrainType::SEA,				TerrainType::SEA,			TerrainType::FOG_OF_WAR},
		{TerrainType::FOG_OF_WAR,	TerrainType::SEA,			TerrainType::DICE_KINGDOM,		TerrainType::SEA,			TerrainType::FOG_OF_WAR},
		{TerrainType::FOG_OF_WAR,	TerrainType::SEA,			TerrainType::SEA,				TerrainType::SEA,			TerrainType::FOG_OF_WAR},
		{TerrainType::FOG_OF_WAR,	TerrainType::FOG_OF_WAR,	TerrainType::FOG_OF_WAR,		TerrainType::FOG_OF_WAR,	TerrainType::FOG_OF_WAR}
	};

	sf::RenderWindow* window;
	sf::Clock deltaClock;
	sf::Time deltaTime = sf::seconds(0.0f), procTime = sf::seconds(1.0f);

	public:
	GameManager(sf::RenderWindow* window, int WIDTH, int HEIGHT);
	void Run();
	void CloseGame();

	void DrawScene();
	void DrawImGui();
	void Proc();
};