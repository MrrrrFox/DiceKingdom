#include "pch.h"
#include "SceneLayout.h"

void DrawCircle3D(float x_center, float y_center, float z_center, float radius, int segments_num)
{
	float x, y, theta;
	glBegin(GL_POLYGON);
	for (int i = 0; i < segments_num; ++i)
	{
		theta = 2.0f * M_PI * float(i) / float(segments_num);

		x = 0.05 * cosf(theta);
		y = 0.05 * sinf(theta);

		glVertex3f(x_center + x, y_center + y, z_center);
	}
	glEnd();
}

SceneLayout::SceneLayout()
{
	int foo = 1;
	char* bar[1] = { (char *)" " };
	glutInit(&foo, bar);

	perspective_projection = false;

	menuTexture.loadFromFile("textures/menu.png");
	menuTexture.generateMipmap();

	unknownTerrainTexture.loadFromFile("textures/terra_incognita.png");
	unknownTerrainTexture.generateMipmap();

	set_hex_size(0.5f);

	drawing_places.insert(std::make_pair("Idle", &SceneLayout::DrawIdle));
	drawing_places.insert(std::make_pair("Lumber Camp", &SceneLayout::DrawLumber));
	drawing_places.insert(std::make_pair("Paint Rig", &SceneLayout::DrawRig));
}

void SceneLayout::initScene(std::pair<unsigned int, unsigned int> window_sizes)
{
	set_width_height(window_sizes.first, window_sizes.second);

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

void SceneLayout::set_width_height(unsigned int width, unsigned int height)
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

void SceneLayout::DrawWorldMap(std::vector<Terrain> world_map)
{
	Spherical playing_camera_north = Spherical(playing_camera.distance, playing_camera.theta, playing_camera.phi + 0.01f);
	gluLookAt(playing_camera.getX(), playing_camera.getY(), playing_camera.getZ(),
		0.0, 0.0, 0.0,
		playing_camera_north.getX(), playing_camera_north.getY(), playing_camera_north.getZ());

	float x_posf, z_posf;
	std::pair<int, int> min_max_x = { 0,0 }, min_max_z = { 0,0 };

	for(auto terrain: world_map)
	{
		if (terrain.position.first > min_max_x.second)
			min_max_x.second = terrain.position.first;
		else if (terrain.position.first < min_max_x.first)
			min_max_x.first = terrain.position.first;

		if (terrain.position.second > min_max_z.second)
			min_max_z.second = terrain.position.second;
		else if (terrain.position.second < min_max_z.first)
			min_max_z.first = terrain.position.second;
	}

	for (auto terrain: world_map)
	{
		//x_posf = (i % 2 ? hex_size.first : hex_size.first / 2.0f) + k * hex_size.first;
		//z_posf = i * 3.0f / 4.0f * hex_size.second;
		if (terrain.visible)
			DrawTerrain(terrain);
		else
			DrawUknown(terrain.position.first, terrain.position.second, min_max_x, min_max_z);
	}
}

void SceneLayout::DrawTerrain(Terrain terrain)
{
	switch (terrain.terrainType)
	{
	case TerrainType::DICE_KINGDOM:
		DrawColorHex(new GLfloat[3] { 0.5f, 0.5f, 0.5f}, terrain.position.first, terrain.position.second);
		DrawCastle(terrain.position.first, terrain.position.second);
		break;
	case TerrainType::SEA:
		DrawColorHex(new GLfloat[3]{ 0.4f, 0.76f, 0.79f }, terrain.position.first, terrain.position.second);
		break;
	case TerrainType::PLAIN:
		DrawColorHex(new GLfloat[3]{ 0.49f, 0.78f, 0.31f }, terrain.position.first, terrain.position.second);
		break;
	default:
		DrawColorHex(new GLfloat[3]{ 0.0f, 0.0f, 0.0f }, terrain.position.first, terrain.position.second);
		throw std::invalid_argument("unknown terrain: " + std::to_string((int)terrain.terrainType));
	}
}

void SceneLayout::DrawUknown(int row, int column, std::pair<int, int> min_max_x, std::pair<int, int> min_max_z)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	sf::Texture::bind(&unknownTerrainTexture);

	int size_x = (min_max_x.second == min_max_x.first ? 1 : min_max_x.second - min_max_x.first),
		size_z = (min_max_z.second == min_max_z.first ? 1 : min_max_z.second - min_max_z.first);
	float tex_hex_h = hex_size.second / (hex_size.second * size_x), tex_hex_w = hex_size.first / (hex_size.first * size_z);

	// it may not work for little maps,
	float	dx = ((float)(row - min_max_x.first) / size_x) * (1.0f - 2 * tex_hex_w) + tex_hex_w,
			dz = ((float)(column - min_max_z.first) / size_z) * (1.0f - 2 * tex_hex_h) + tex_hex_h;

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + column * hex_size.first, 0.0, row * 3.0f / 4.0f * hex_size.second);
	glBegin(GL_POLYGON);
	glTexCoord2f(dx,				dz-tex_hex_h/2);		glVertex3f(0.0f,				0.0f, -hex_size.second / 2);
	glTexCoord2f(dx - tex_hex_w/2,	dz - tex_hex_h / 4);	glVertex3f(-hex_size.first / 2, 0.0f, -hex_size.second / 4);
	glTexCoord2f(dx - tex_hex_w/2,	dz + tex_hex_h / 4);	glVertex3f(-hex_size.first / 2, 0.0f, hex_size.second / 4);
	glTexCoord2f(dx,				dz+ tex_hex_h / 2);		glVertex3f(0.0f,				0.0f, hex_size.second / 2);
	glTexCoord2f(dx + tex_hex_w/2,	dz + tex_hex_h / 4);	glVertex3f(hex_size.first / 2,	0.0f, hex_size.second / 4);
	glTexCoord2f(dx + tex_hex_w/2,	dz - tex_hex_h / 4);	glVertex3f(hex_size.first / 2,	0.0f, -hex_size.second / 4);
	glEnd();
	glPopMatrix();
}

std::pair<float, float> SceneLayout::CalculateHexPosition(int row, int column)
{
	float x_pos = (row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + column * hex_size.first;
	float z_pos = row * 3.0f / 4.0f * hex_size.second;
	return std::pair<float, float>(x_pos, z_pos);
}

void SceneLayout::DrawColorHex(GLfloat * color, int row, int column)
{
	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + column * hex_size.first, 0.0, row * 3.0f / 4.0f * hex_size.second);
	glBegin(GL_POLYGON);
	glColor3fv(color);	glVertex3f(0.0f,					0.0f, - hex_size.second / 2);
	glColor3fv(color);  glVertex3f(- hex_size.first / 2,	0.0f, - hex_size.second / 4);
	glColor3fv(color);  glVertex3f(- hex_size.first / 2,	0.0f, hex_size.second / 4);
	glColor3fv(color);  glVertex3f(0.0f,					0.0f, hex_size.second / 2);
	glColor3fv(color);  glVertex3f(hex_size.first / 2,		0.0f, hex_size.second / 4);
	glColor3fv(color);  glVertex3f(hex_size.first / 2,		0.0f, - hex_size.second / 4);
	glEnd();
	glPopMatrix();
	delete[] color;
}

void SceneLayout::DrawKingdom(std::map<std::string, PlaceWithLimitedInformation> places, float time_to_proc)
{	
	Spherical playing_camera_north = Spherical(playing_camera.distance, playing_camera.theta, playing_camera.phi + 0.01f);
	gluLookAt(playing_camera.getX(), playing_camera.getY(), playing_camera.getZ(),
		0.0, 0.0, 0.0,
		playing_camera_north.getX(), playing_camera_north.getY(), playing_camera_north.getZ());

	// draw places
	for (auto place = places.begin(); place!= places.end(); ++place)
	{
		(this->*drawing_places[place->first])();
	}
	DrawColorHex(new GLfloat[3]{ 0.49f, 0.78f, 0.31f }, -1, 0);
	DrawColorHex(new GLfloat[3]{ 0.49f, 0.78f, 0.31f }, -1, -1);
	DrawColorHex(new GLfloat[3]{ 0.49f, 0.78f, 0.31f }, -1, 1);
	DrawColorHex(new GLfloat[3]{ 0.49f, 0.78f, 0.31f }, 1, -1);
	DrawColorHex(new GLfloat[3]{ 0.4f, 0.76f, 0.79f }, 0, 1);
	DrawColorHex(new GLfloat[3]{ 0.4f, 0.76f, 0.79f }, 1, 1);

	// draw last rolls
	float colors[4] = { 0.0, 0.0, 0.0, time_to_proc };
	for (auto place = places.begin(); place != places.end(); ++place)
	{
		if (place->second.last_roll > 0)
		{
			std::pair<float, float> pos = CalculateHexPosition(place->second.position.first, place->second.position.second);
			float position[3] = { pos.first, 0.4, pos.second };
			RenderString(position, GLUT_BITMAP_HELVETICA_18, (const unsigned char*)std::to_string(place->second.last_roll).data(), colors);
		}
	}


}

void SceneLayout::DrawIdle()
{
	int logs = 6, row = 0, column = 0;
	DrawColorHex(new GLfloat[3]{ 0.76f, 0.7f, 0.5f }, 0, 0);

	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + column * hex_size.first, 0.025, row * 3.0f / 4.0f * hex_size.second);

	// wood
	double eqn[4] = { 0.0, 1.0, 0.0, 0.025 };
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);

	for (int i = 0; i < logs; ++i)
	{
		glPushMatrix();
		glRotatef(i*60.0, 0.0, 1.0, 0.0);
		glColor3f(0.55f, 0.38f, 0.26f);
		gluCylinder(qobj, 0.025, 0.05, 0.2, 15, 5);
		DrawCircle3D(0.0, 0.0, 0.2, 0.05, 20);
		glPopMatrix();
	}
	glDisable(GL_CLIP_PLANE0);

	// fire
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 0.1, 0.0, 0.3, 15, 5);
	glPopMatrix();

	glPopMatrix();
}

void SceneLayout::DrawLumber()
{
	int tmp, row = 0, column = -1;
	DrawColorHex(new GLfloat[3]{ 0.49f, 0.78f, 0.31f }, row, column);

	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	float wall_sizes[2] = { hex_size.first / 4, hex_size.first / 6 };

	float walls_offsets[4][2] = { {0.0, wall_sizes[1]},
								{wall_sizes[0], 0.0},
								{0.0, -wall_sizes[1]},
								{-wall_sizes[0], 0.0} };

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + column * hex_size.first - hex_size.first / 8, 0.0, row * 3.0f / 4.0f * hex_size.second - hex_size.second / 8);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	
	//walls
	for (int i = 0; i < 4; ++i)
	{
		tmp = i % 2;
		glPushMatrix();
		glColor3f(0.73f, 0.55f, 0.39f);
		glTranslatef(walls_offsets[i][0], 0.0, walls_offsets[i][1]);

		if (tmp == 1) glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex3f(-wall_sizes[tmp], 0.2, 0.0f);
		glVertex3f(-wall_sizes[tmp], 0.0, 0.0f);
		glVertex3f(wall_sizes[tmp], 0.0, 0.0f);
		glVertex3f(wall_sizes[tmp], 0.2, 0.0f);
		if (tmp == 1) glVertex3f(0.0, 0.2+wall_sizes[1], 0.0f);
		glEnd();
		
		glPopMatrix();
	}
	// roof
	for (int i = 0; i < 2; ++i)
	{
		tmp = i % 2;
		glPushMatrix();
		glColor3f(0.55f, 0.38f, 0.26f);
		glTranslatef(0.0, 0.2, (tmp==0 ? wall_sizes[1] : -wall_sizes[1]));

		glRotatef((tmp == 0 ? -45.0 : 45.0), 1.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-wall_sizes[0] - 0.05, 0.22, 0.0f);
		glVertex3f(-wall_sizes[0] - 0.05, -0.05, 0.0f);
		glVertex3f(wall_sizes[0] + 0.05, -0.05, 0.0f);
		glVertex3f(wall_sizes[0] + 0.05, 0.22, 0.0f);
		glEnd();

		glPopMatrix();
	}
	
	glPushMatrix();
	glTranslatef(0.0, 0.0, hex_size.second / 3);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	glRotatef(180.0, 0.0, 0.0, 1.0);
		// trunk
		glColor3f(0.55f, 0.38f, 0.26f);
		gluCylinder(qobj, 0.05, 0.05, 0.05, 15, 5);
	
		glColor3f(0.73f, 0.55f, 0.39f);
		DrawCircle3D(0.0f, 0.0f, 0.05f, 0.05f, 20);

		// axe
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.07);
		glRotatef(-60.0, 0.0, 1.0, 0.0);
		glColor3f(0.75f, 0.75f, 0.75f);
		glBegin(GL_QUADS);
		glVertex3f(-0.03, 0.0, 0.02);
		glVertex3f(-0.03, 0.0, -0.02);
		glVertex3f(0.03, 0.0, -0.02);
		glVertex3f(0.03, 0.0, 0.02);
		glEnd();

			
			glPushMatrix();
			glTranslatef(0.03, 0.0, -0.02);
			glColor3f(0.55f, 0.38f, 0.26f);
			gluCylinder(qobj, 0.005, 0.005, 0.15, 15, 5);		
			glPopMatrix();
		glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void SceneLayout::DrawRig()
{
	int row = 1, column = 0;
	DrawColorHex(new GLfloat[3]{ 0.4f, 0.76f, 0.79f }, row, column);

	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	float leg_offset = hex_size.second / 10;
	float rig_half_size = leg_offset + 0.05;

	float legs_offsets[4][2] = { {leg_offset, leg_offset},
								 {leg_offset, -leg_offset},
								 {-leg_offset, leg_offset},
								 {-leg_offset, -leg_offset} };
	float walls_offsets[4][2] = { {0.0, rig_half_size},
								{rig_half_size, 0.0},
								{0.0, -rig_half_size},
								{-rig_half_size, 0.0} };
	float windows_offsets[4][2] = { {0.0, rig_half_size+10e-6},
							{rig_half_size+ 10e-6, 0.0},
							{0.0, -rig_half_size-10e-6},
							{-rig_half_size-10e-6, 0.0} };

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + column * hex_size.first, 0.0, row * 3.0f / 4.0f * hex_size.second);

	for (int i = 0; i < 4; ++i)
	{
		// leg
		glPushMatrix();
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(legs_offsets[i][0], 0.0, legs_offsets[i][1]);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(qobj, 0.03, 0.03, 0.15, 15, 5);
		glPopMatrix();

		// wall
		glPushMatrix();
		glColor3f(0.41f, 0.41f, 0.41f);
		glTranslatef(walls_offsets[i][0], 0.15, walls_offsets[i][1]);
		if (i % 2) glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-rig_half_size, 0.1, 0.0f);
		glVertex3f(-rig_half_size, 0.0, 0.0f);
		glVertex3f(rig_half_size, 0.0, 0.0f);
		glVertex3f(rig_half_size, 0.1, 0.0f);
		glEnd();
		glPopMatrix();

		// window
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(windows_offsets[i][0], 0.175, windows_offsets[i][1]);
		if (i % 2) glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-rig_half_size / 2, 0.05, 0.0f);
		glVertex3f(-rig_half_size / 2, 0.0, 0.0f);
		glVertex3f(rig_half_size / 2, 0.0, 0.0f);
		glVertex3f(rig_half_size / 2, 0.05, 0.0f);
		glEnd();
		glPopMatrix();
	}
	glColor3f(0.41f, 0.41f, 0.41f);
	for (int i = 0; i < 2; ++i)
	{
		// floor / roof 
		glPushMatrix();
		glTranslatef(0.0, 0.15 + i*0.10, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-rig_half_size, 0.0, -rig_half_size);
		glVertex3f(-rig_half_size, 0.0, rig_half_size);
		glVertex3f(rig_half_size, 0.0, rig_half_size);
		glVertex3f(rig_half_size, 0.0, -rig_half_size);
		glEnd();
		glPopMatrix();
	}
	// stacks
	glColor3f(0.5f, 0.5f, 0.5f);
	for (int i = 0; i < 3; ++i)
	{
		glPushMatrix();
		glTranslatef(2*rig_half_size/3, 0.25, -rig_half_size/2 + i* rig_half_size/2);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(qobj, 0.02, 0.02, 0.05, 15, 5);
		glPopMatrix();
	}

	glPopMatrix();
}


void SceneLayout::DrawCastle(int row, int column)
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
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + column * hex_size.first, 0.0, row * 3.0f / 4.0f * hex_size.second);

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
	glColor4f(colors[0], colors[1], colors[2], colors[3]);
	glRasterPos3f(position[0], position[1], position[2]);

	/*const unsigned char* c;

	for (c = string; *c != '\0'; ++c)
	{
		glutStrokeCharacter(font, *c);
	}*/

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