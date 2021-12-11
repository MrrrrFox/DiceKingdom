#pragma once
#include "enums.h"
#include "GraphicsManager.h"
#include "DiceKingdom.h"


class GameManager
{
	GameView currentView;
	bool isRunning;
	bool isPlaying;
	int WIDTH;
	int HEIGHT;

	GraphicsManager GM;
	DiceKingdom DK;

	std::vector<std::vector<WorldTerrain>> world_map;

	sf::RenderWindow * window;
	sf::Clock deltaClock;
	sf::Time deltaTime;

public:
	GameManager(sf::RenderWindow * window, int WIDTH, int HEIGHT);
	void Run();
	void DrawScene();
	void DrawImGui();
	void Proc();
	void CloseGame();

	void initOpenGL();
	void reshapeScreen(sf::Vector2u size);
};