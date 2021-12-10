#include "pch.h"
#include "GameManager.h"

GameManager::GameManager()
{
	currentView = GameView::MENU;
	isRunning = true;
	isPlaying = false;

	world_map = { {WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,		WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::SEA,				WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::DICE_KINGDOM,		WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::SEA,				WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,		WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR} };
}

void GameManager::Run()
{
	while (isRunning)
	{
		DrawScene();

		// events

		if (isPlaying)
		{
			// proc actions
		}
		break;
	}
}

void GameManager::DrawScene()
{
	currentView = GameView::KINGDOM_LUMBER;
	switch (currentView)
	{
	case GameView::MENU:
		GM.DrawMenu();
		break;
	case GameView::MAP:
		GM.DrawWorldMap(world_map);
		break;
	case GameView::KINGDOM:
	case GameView::KINGDOM_LUMBER:
	case GameView::KINGDOM_RIG:
		GM.DrawKingdom(DK.get_places(), currentView);
		break;
	default:
		std::cerr << "unknown scene to draw";
	}
}