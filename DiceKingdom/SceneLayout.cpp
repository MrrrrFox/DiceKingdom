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

	unknownTerrainTexture.loadFromFile("textures/terra_incognita.png");
	unknownTerrainTexture.generateMipmap();

	set_hex_size(0.5f);
}

void SceneLayout::initScene(std::pair<int, int> window_sizes, std::pair<int, int> DK_pos)
{
	set_width_height(window_sizes.first, window_sizes.second);
	set_map_origin(DK_pos.first, DK_pos.second);

	reshapeScreen(sf::Vector2u(WIDTH,HEIGHT));

	// initialize OpenGL
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat light_ambient_global[4] = { 0.5,0.5,0.5,1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient_global);
}

void SceneLayout::reshapeScreen(sf::Vector2u size)
{
	glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (perspective_projection) gluPerspective(45.0f, (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
	else glOrtho(-((GLdouble)size.x / (GLdouble)size.y), ((GLdouble)size.x / (GLdouble)size.y), -1.0, 1.0, -3.0, 12.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

void SceneLayout::set_map_origin(int DK_pos_x, int DK_pos_z)
{
	map_origin.first = - ((DK_pos_x % 2 ? hex_size.first : hex_size.first / 2) + DK_pos_z * hex_size.first);
	map_origin.second = - (hex_size.second/2.0f + DK_pos_x * 3.0f / 4.0f * hex_size.second);
}

void SceneLayout::set_hex_size(float _size)
{
	hex_size.first = (float)sqrt(3) * _size;
	hex_size.second = 2.0f * _size;
}

void SceneLayout::DrawMenu()
{
	Spherical menu_camera_north = Spherical(menu_camera.distance, menu_camera.theta, menu_camera.phi + 0.01f);
	gluLookAt(	menu_camera.getX(), menu_camera.getY(), menu_camera.getZ(),
				0.0, 0.0, 0.0,
				menu_camera_north.getX(), menu_camera_north.getY(), menu_camera_north.getZ());

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

void SceneLayout::DrawWorldMap(std::vector<std::vector<Terrain>> world_map)
{
	Spherical map_camera_north = Spherical(map_camera.distance, map_camera.theta, map_camera.phi + 0.01f);
	gluLookAt(map_camera.getX(), map_camera.getY(), map_camera.getZ(),
		0.0, 0.0, 0.0,
		map_camera_north.getX(), map_camera_north.getY(), map_camera_north.getZ());

	float x_posf, z_posf;
	int max_x, max_z, tmp;

	max_x = (int)world_map.size();
	max_z = (int)world_map[0].size();
	for(auto row: world_map)
	{
		tmp = (int)row.size();
		if (tmp > max_z)
			max_z = tmp;
	}

	glPushMatrix();
	glTranslatef(map_origin.first, 0.0f, map_origin.second);

	for (int i = 0; i < world_map.size(); ++i)
	{
		for (int k = 0; k < world_map[i].size(); ++k)
		{
			x_posf = (i % 2 ? hex_size.first : hex_size.first / 2.0f) + k * hex_size.first;
			z_posf = i * 3.0f / 4.0f * hex_size.second;
			if (world_map[i][k].visible)
				DrawTerrain(world_map[i][k].terrainType, x_posf, z_posf);
			else
				DrawUknown(x_posf, z_posf, max_x, max_z);
		}
	}
			
	glPopMatrix();
}

void SceneLayout::DrawTerrain(TerrainType terrain, float x_posf, float z_posf)
{
	switch (terrain)
	{
	case TerrainType::DICE_KINGDOM:
		DrawColorHex(new GLfloat[3] { 0.5f, 0.5f, 0.5f}, x_posf, z_posf);
		DrawCastle(x_posf, z_posf);
		break;
	case TerrainType::SEA:
		DrawColorHex(new GLfloat[3]{ 0.4f, 0.76f, 0.79f }, x_posf, z_posf);
		break;
	case TerrainType::PLAIN:
		DrawColorHex(new GLfloat[3]{ 0.49f, 0.78f, 0.31f }, x_posf, z_posf);
		break;
	default:
		DrawColorHex(new GLfloat[3]{ 0.0f, 0.0f, 0.0f }, x_posf, z_posf);
		std::cerr << "unknown terrain: " << (int)terrain << std::endl;
	}
}

void SceneLayout::DrawUknown(float x_posf, float z_posf, int x_map_size, int z_map_size)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	sf::Texture::bind(&unknownTerrainTexture);

	float dx = x_posf / x_map_size, dz = z_posf / z_map_size;

	float tex_hex_h = hex_size.second / (hex_size.second * x_map_size), tex_hex_w = hex_size.first / (hex_size.first * z_map_size);

	glBegin(GL_POLYGON);
	glTexCoord2f(dx, dz);										glVertex3f(x_posf, 0.0f, z_posf);
	glTexCoord2f(dx - tex_hex_w / 2, dz + tex_hex_h / 4);		glVertex3f(x_posf - hex_size.first / 2, 0.0f, z_posf + hex_size.second / 4);
	glTexCoord2f(dx - tex_hex_w / 2, dz + 3 * tex_hex_h / 4);	glVertex3f(x_posf - hex_size.first / 2, 0.0f, z_posf + 3 * hex_size.second / 4);
	glTexCoord2f(dx, dz + tex_hex_h);							glVertex3f(x_posf, 0.0f, z_posf + hex_size.second);
	glTexCoord2f(dx + tex_hex_w / 2, dz + 3 * tex_hex_h / 4);	glVertex3f(x_posf + hex_size.first / 2, 0.0f, z_posf + 3 * hex_size.second / 4);
	glTexCoord2f(dx + tex_hex_w / 2, dz + tex_hex_h / 4);		glVertex3f(x_posf + hex_size.first / 2, 0.0f, z_posf + hex_size.second / 4);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void SceneLayout::DrawColorHex(GLfloat * color, float x_posf, float z_posf)
{
	glBegin(GL_POLYGON);
	glColor3fv(color); glVertex3f(x_posf, 0.0f, z_posf);
	glColor3fv(color); glVertex3f(x_posf, 0.0f, z_posf);
	glColor3fv(color);  glVertex3f(x_posf - hex_size.first / 2, 0.0f, z_posf + hex_size.second / 4);
	glColor3fv(color);  glVertex3f(x_posf - hex_size.first / 2, 0.0f, z_posf + 3 * hex_size.second / 4);
	glColor3fv(color);  glVertex3f(x_posf, 0.0f, z_posf + hex_size.second);
	glColor3fv(color);  glVertex3f(x_posf + hex_size.first / 2, 0.0f, z_posf + 3 * hex_size.second / 4);
	glColor3fv(color);  glVertex3f(x_posf + hex_size.first / 2, 0.0f, z_posf + hex_size.second / 4);
	glEnd();
	delete[] color;
}

void SceneLayout::DrawKingdom()
{
	Spherical kingdom_camera_north = Spherical(kingdom_camera.distance, kingdom_camera.theta, kingdom_camera.phi + 0.01f);
	gluLookAt(kingdom_camera.getX(), kingdom_camera.getY(), kingdom_camera.getZ(),
		0.0, 0.0, 0.0,
		kingdom_camera_north.getX(), kingdom_camera_north.getY(), kingdom_camera_north.getZ());

	//uklad
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
	glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, 1.0, 0);
	glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();

	//Linie przerywane
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0xAAAA);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(-1.0, 0, 0);
	glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, -1.0, 0);
	glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, -1.0);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

void SceneLayout::DrawCastle(float x_posf, float z_posf)
{
	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	float mini_tower_offset = hex_size.second / 4;

	float mini_tower_offsets[4][2] = { {mini_tower_offset, mini_tower_offset},
										{mini_tower_offset, -mini_tower_offset},
										{-mini_tower_offset, mini_tower_offset},
										{-mini_tower_offset, -mini_tower_offset} };
	float walls_offsets[4][2] = {	{0.0, mini_tower_offset},
									{mini_tower_offset, 0.0},
									{0.0, -mini_tower_offset},
									{-mini_tower_offset, 0.0} };

	glPushMatrix();
	glTranslatef(x_posf, 0.0, z_posf + hex_size.second / 2);

	// main-tower
	glPushMatrix();
	glColor3f(0.41f, 0.41f, 0.41f);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluCylinder(qobj, 0.15, 0.15, 0.4, 15, 5);
	// main-tower's roof
	glColor3f(0.74f, 0.29f, 0.24f);
	glTranslatef(0.0, 0.0, 0.4);
	gluCylinder(qobj, 0.2, 0.0, 0.2, 15, 5);
	glPopMatrix();

	for (int i = 0; i < 4; ++i)
	{
		// mini-tower
		glPushMatrix();
		glColor3f(0.82f, 0.82f, 0.82f);
		glTranslatef(mini_tower_offsets[i][0], 0.0, mini_tower_offsets[i][1]);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(qobj, 0.075, 0.075, 0.2, 15, 5);
		// mini-tower's roof
		glColor3f(0.74f, 0.29f, 0.24f);
		glTranslatef(0.0, 0.0, 0.2);
		gluCylinder(qobj, 0.1, 0.0, 0.1, 15, 5);
		glPopMatrix();

		// wall
		glPushMatrix();
		glColor3f(0.66f, 0.66f, 0.66f);
		glTranslatef(walls_offsets[i][0], 0.0, walls_offsets[i][1]);
		if(i%2) glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-mini_tower_offset, 0.18, 0.0f);
		glVertex3f(-mini_tower_offset, 0.0, 0.0f);
		glVertex3f(mini_tower_offset, 0.0, 0.0f);
		glVertex3f(mini_tower_offset, 0.18, 0.0f);
		glEnd();
		glPopMatrix();
	}



	
	glPopMatrix();
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