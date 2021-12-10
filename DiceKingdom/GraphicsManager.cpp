#include "pch.h"
#include "GraphicsManager.h"

void GraphicsManager::DrawMenu()
{
	std::cout << "MENU" << std::endl;
}

void GraphicsManager::DrawWorldMap(std::vector<std::vector<WorldTerrain>> world_map)
{
	for (int i = 0; i < world_map.size(); ++i)
	{
		for (int j = 0; j < world_map[i].size(); ++j)
			DrawTerrain(world_map[i][j], i, j);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void GraphicsManager::DrawTerrain(WorldTerrain terrain, int x_pos, int y_pos)
{
	switch (terrain)
	{
	case WorldTerrain::DICE_KINGDOM:
		std::cout << " K ";
		break;
	case WorldTerrain::SEA:
		std::cout << " ~ ";
		break;
	case WorldTerrain::FOG_OF_WAR:
		std::cout << " # ";
		break;
	default:
		std::cerr << "undefined terrain type to draw";
	}
}

void GraphicsManager::DrawKingdom(std::vector<Place *> places, GameView game_view)
{
	for(int i=0; i<places.size(); ++i)
		std::cout << places[i]->get_name() << " ";
	std::cout << std::endl;
	//switch (game_view)
	//{
	//case GameView::KINGDOM_LUMBER:
	//	std::cout << "SHOWING LUMBER PANEL" << std::endl;
	//	break;
	////case GameView::KINGDOM_RIG:
	////	std::cout << "SHOWING LUMBER PANEL" << std::endl;
	////	break;
	//}
	if (game_view == GameView::KINGDOM_LUMBER)
		std::cout << "SHOWING LUMBER PANEL" << std::endl;
	else if (game_view == GameView::KINGDOM_RIG)
		std::cout << "SHOWING RIG PANEL" << std::endl;

}