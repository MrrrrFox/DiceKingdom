#include "pch.h"
#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
	menuTexture.loadFromFile("textures/menu.png");
	menuTexture.generateMipmap();
}

void GraphicsManager::DrawMenu()
{
	//std::cout << "MENU" << std::endl;
	// 
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glEnable(GL_TEXTURE_2D);
	//sf::Texture::bind(&menuTexture);

	//glBegin(GL_QUADS);
	//for (int x = 1; x < 18; x++)
	//	for (int y = 1; y < 18; y++)
	//	{
	//		//flag[][].x -> <-1;1>
	//		//flag[][].z -> <-1;1>
	//		glTexCoord2f((flag[x][y].x + 1.0f) / 2.0f, (flag[x][y].z + 1.0f) / 2.0f);
	//		glNormalsf(flag_n[x][y]);         glVertexsf(flag[x][y]);
	//		glTexCoord2f((flag[x + 1][y].x + 1.0f) / 2.0f, (flag[x + 1][y].z + 1.0f) / 2.0f);
	//		glNormalsf(flag_n[x + 1][y]);     glVertexsf(flag[x + 1][y]);
	//		glTexCoord2f((flag[x + 1][y + 1].x + 1.0f) / 2.0f, (flag[x + 1][y + 1].z + 1.0f) / 2.0f);
	//		glNormalsf(flag_n[x + 1][y + 1]); glVertexsf(flag[x + 1][y + 1]);
	//		glTexCoord2f((flag[x][y + 1].x + 1.0f) / 2.0f, (flag[x][y + 1].z + 1.0f) / 2.0f);
	//		glNormalsf(flag_n[x][y + 1]);     glVertexsf(flag[x][y + 1]);
	//	}
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
}

void GraphicsManager::DrawWorldMap(std::vector<std::vector<WorldTerrain>> world_map)
{
	//for (int i = 0; i < world_map.size(); ++i)
	//{
	//	for (int j = 0; j < world_map[i].size(); ++j)
	//		DrawTerrain(world_map[i][j], i, j);
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;
}

void GraphicsManager::DrawTerrain(WorldTerrain terrain, int x_pos, int y_pos)
{
	//switch (terrain)
	//{
	//case WorldTerrain::DICE_KINGDOM:
	//	std::cout << " K ";
	//	break;
	//case WorldTerrain::SEA:
	//	std::cout << " ~ ";
	//	break;
	//case WorldTerrain::FOG_OF_WAR:
	//	std::cout << " # ";
	//	break;
	//default:
	//	std::cerr << "undefined terrain type to draw";
	//}
}

void GraphicsManager::DrawKingdom(std::vector<Place *> places, GameView game_view)
{
	/*for(int i=0; i<places.size(); ++i)
		std::cout << places[i]->get_name() << " ";
	std::cout << std::endl;*/
	if (game_view == GameView::KINGDOM_LUMBER)
	{
		//std::cout << "SHOWING LUMBER PANEL" << std::endl;
	}
	else if (game_view == GameView::KINGDOM_RIG)
	{
		//std::cout << "SHOWING RIG PANEL" << std::endl;
	}
}