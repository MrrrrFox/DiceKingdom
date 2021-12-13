#include "pch.h"
#include "SceneLayout.h"

SceneLayout::SceneLayout()
{
	//int foo = 1;
	//char* bar[1] = { (char *)" " };
	//glutInit(&foo, bar);

	perspective_projection = false;

	menuTexture.loadFromFile("textures/menu.png");
	menuTexture.generateMipmap();
}

bool SceneLayout::get_perspective_projection()
{
	return perspective_projection;
}

void SceneLayout::set_perspective_projection(bool _perspective_projection)
{
	perspective_projection = _perspective_projection;
}

void SceneLayout::set_width_height(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	width_to_height = (float) WIDTH / HEIGHT;
}

void SceneLayout::DrawMenu()
{
	gluLookAt(	menu_camera.getX(), menu_camera.getY(), menu_camera.getZ(),
				0.0, 0.0, 0.0,
				menu_camera.getX(0.01f), menu_camera.getY(0.01f), menu_camera.getZ(0.01f));

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	sf::Texture::bind(&menuTexture);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-width_to_height, -1.0, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-width_to_height, 1.0, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width_to_height, 1.0, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width_to_height, -1.0, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void SceneLayout::DrawWorldMap(std::vector<std::vector<WorldTerrain>> world_map)
{
	//for (int i = 0; i < world_map.size(); ++i)
	//{
	//	for (int j = 0; j < world_map[i].size(); ++j)
	//		DrawTerrain(world_map[i][j], i, j);
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;
}

void SceneLayout::DrawTerrain(WorldTerrain terrain, int x_pos, int y_pos)
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

void SceneLayout::DrawKingdom(std::vector<Place *> places, GameView game_view)
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

void SceneLayout::RenderString(float position[], void * font, const unsigned char * string, float colors[])
{
	glColor3f(colors[0], colors[1], colors[2]);
	glRasterPos3f(position[0], position[1], position[2]);

	glutBitmapString(font, string);
}

//void GraphicsManager::drawText(const char* text, int text_length, int x_pos, int y_pos)
//{
//	glMatrixMode(GL_PROJECTION);
//	double* matrix = new double[16];
//	glGetDoublev(GL_PROJECTION, matrix);
//	glLoadIdentity();
//	glOrtho(0, (GLdouble)WIDTH, 0, (GLdouble)HEIGHT, -5, 5);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glPushMatrix();
//	glLoadIdentity();
//	glRasterPos2i(x_pos, y_pos);
//	for (int i = 0; i < text_length; ++i)
//	{
//		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
//	}
//	glPopMatrix();
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixd(matrix);
//	glMatrixMode(GL_MODELVIEW);
//
//}