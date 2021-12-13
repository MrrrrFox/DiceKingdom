#pragma once
#include "enums.h"
#include "SceneLayout.h"
#include "DiceKingdom.h"


class GameManager
{
	GameView currentView;
	bool isRunning, isPlaying;
	int WIDTH = 600, HEIGHT = 400;

	SceneLayout sceneLayout;
	DiceKingdom DK;

	std::vector<std::vector<WorldTerrain>> world_map;

	sf::RenderWindow * window;
	sf::Clock deltaClock;
	sf::Time deltaTime;
	sf::Time procTime;

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