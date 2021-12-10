#pragma once
#include "enums.h"
#include "GraphicsManager.h"
#include "DiceKingdom.h"


class GameManager
{
	GameView currentView;
	bool isRunning;
	bool isPlaying;

	GraphicsManager GM;
	DiceKingdom DK;

	std::vector<std::vector<WorldTerrain>> world_map;

public:
	GameManager();
	void Run();
	void DrawScene();
};