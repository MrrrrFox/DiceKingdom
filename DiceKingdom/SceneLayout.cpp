#include "pch.h"
#include "SceneLayout.h"

void DrawCircle3D(float x_center, float y_center, float z_center, float radius, int segments_num)
{
	float x;
	float y;
	float theta;
	glBegin(GL_POLYGON);
	for(int i = 0; i < segments_num; ++i)
	{
		theta = 2.0f * static_cast<float> (M_PI) * static_cast<float> (i) / static_cast<float> (segments_num);

		x = radius * cosf(theta);
		y = radius * sinf(theta);

		glVertex3f(x_center + x, y_center + y, z_center);
	}
	glEnd();
}

SceneLayout::SceneLayout()
{
	menuTexture.loadFromFile("textures/menu.png");
	menuTexture.generateMipmap();

	unknownTerrainTexture.loadFromFile("textures/terra_incognita.png");
	unknownTerrainTexture.generateMipmap();

	set_hex_size(0.5f);

	drawing_places.try_emplace("Idle", &SceneLayout::DrawIdle);
	drawing_places.try_emplace("Lumber Camp", &SceneLayout::DrawLumber);
	drawing_places.try_emplace("Paint Rig", &SceneLayout::DrawRig);
}

void SceneLayout::initScene(std::pair<unsigned int, unsigned int> window_sizes)
{
	set_width_height(window_sizes.first, window_sizes.second);

	reshapeScreen(sf::Vector2u(WIDTH, HEIGHT));

	// initialize OpenGL
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	std::array<GLfloat, 4> light_ambient_global = {0.5,0.5,0.5,1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient_global._Elems);
}

void SceneLayout::reshapeScreen(sf::Vector2u size) const
{
	glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(perspective_projection)
		gluPerspective(45.0f, (GLdouble) size.x / (GLdouble) size.y, 0.1, 100.0);
	else
		glOrtho(-((GLdouble) size.x / (GLdouble) size.y), ((GLdouble) size.x / (GLdouble) size.y), -1.0, 1.0, -3.0, 12.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool SceneLayout::get_perspective_projection() const
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
	width_to_height = static_cast<float> (WIDTH) / static_cast<float> (HEIGHT);
}

void SceneLayout::set_map_origin(int DK_pos_x, int DK_pos_z)
{
	map_origin.first = -((DK_pos_x % 2 ? hex_size.first : hex_size.first / 2) + static_cast<float> (DK_pos_z) * hex_size.first);
	map_origin.second = -(hex_size.second / 2.0f + static_cast<float> (DK_pos_x) * 3.0f / 4.0f * hex_size.second);
}

void SceneLayout::set_hex_size(float _size)
{
	hex_size.first = (float) sqrt(3) * _size;
	hex_size.second = 2.0f * _size;
}

Spherical* SceneLayout::get_playing_camera()
{
	return &playing_camera;
}

void SceneLayout::DrawMenu() const
{
	auto menu_camera_north = Spherical(menu_camera.distance, menu_camera.theta, menu_camera.phi + 0.01f);
	gluLookAt(menu_camera.getX(), menu_camera.getY(), menu_camera.getZ(),
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

void SceneLayout::DrawWorldMap(const std::vector<Terrain>& world_map) const
{
	auto playing_camera_north = Spherical(playing_camera.distance, playing_camera.theta, playing_camera.phi + 0.01f);
	gluLookAt(playing_camera.getX(), playing_camera.getY(), playing_camera.getZ(),
			  0.0, 0.0, 0.0,
			  playing_camera_north.getX(), playing_camera_north.getY(), playing_camera_north.getZ());

	std::pair<int, int> min_max_x = {0,0};
	std::pair<int, int> min_max_z = {0,0};

	for(auto terrain : world_map)
	{
		if(terrain.position.first > min_max_x.second)
			min_max_x.second = terrain.position.first;
		else if(terrain.position.first < min_max_x.first)
			min_max_x.first = terrain.position.first;

		if(terrain.position.second > min_max_z.second)
			min_max_z.second = terrain.position.second;
		else if(terrain.position.second < min_max_z.first)
			min_max_z.first = terrain.position.second;
	}

	for(auto terrain : world_map)
	{
		if(terrain.visible)
			DrawTerrain(terrain);
		else
			DrawUknown(terrain.position.first, terrain.position.second, min_max_x, min_max_z);
	}
}

void SceneLayout::DrawTerrain(Terrain terrain) const
{
	switch(terrain.terrainType)
	{
		case TerrainType::DICE_KINGDOM:
			DrawColorHex({0.5f, 0.5f, 0.5f}, terrain.position.first, terrain.position.second);
			DrawCastle(terrain.position.first, terrain.position.second);
			break;
		case TerrainType::SEA:
			DrawColorHex({0.4f, 0.76f, 0.79f}, terrain.position.first, terrain.position.second);
			break;
		case TerrainType::PLAIN:
			DrawColorHex({0.49f, 0.78f, 0.31f}, terrain.position.first, terrain.position.second);
			break;
		default:
			DrawColorHex({0.0f, 0.0f, 0.0f}, terrain.position.first, terrain.position.second);
			throw std::invalid_argument("unknown terrain: " + std::to_string((int) terrain.terrainType));
	}
}

void SceneLayout::DrawUknown(int row, int column, std::pair<int, int> min_max_x, std::pair<int, int> min_max_z) const
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	sf::Texture::bind(&unknownTerrainTexture);

	int size_x = (min_max_x.second == min_max_x.first ? 1 : min_max_x.second - min_max_x.first);
	int size_z = (min_max_z.second == min_max_z.first ? 1 : min_max_z.second - min_max_z.first);
	float tex_hex_h = hex_size.second / (hex_size.second * static_cast<float> (size_x));
	float tex_hex_w = hex_size.first / (hex_size.first * static_cast<float> (size_z));

	// it may not work for little maps,
	float	dx = (static_cast<float> (row - min_max_x.first) / static_cast<float> (size_x)) * (1.0f - 2 * tex_hex_w) + tex_hex_w;
	float dz = (static_cast<float> (column - min_max_z.first) / static_cast<float> (size_z)) * (1.0f - 2 * tex_hex_h) + tex_hex_h;

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + static_cast<float> (column) * hex_size.first, 0.0, static_cast<float> (row) * 3.0f / 4.0f * hex_size.second);
	glBegin(GL_POLYGON);
	glTexCoord2f(dx, dz - tex_hex_h / 2);					glVertex3f(0.0f, 0.0f, -hex_size.second / 2);
	glTexCoord2f(dx - tex_hex_w / 2, dz - tex_hex_h / 4);	glVertex3f(-hex_size.first / 2, 0.0f, -hex_size.second / 4);
	glTexCoord2f(dx - tex_hex_w / 2, dz + tex_hex_h / 4);	glVertex3f(-hex_size.first / 2, 0.0f, hex_size.second / 4);
	glTexCoord2f(dx, dz + tex_hex_h / 2);					glVertex3f(0.0f, 0.0f, hex_size.second / 2);
	glTexCoord2f(dx + tex_hex_w / 2, dz + tex_hex_h / 4);	glVertex3f(hex_size.first / 2, 0.0f, hex_size.second / 4);
	glTexCoord2f(dx + tex_hex_w / 2, dz - tex_hex_h / 4);	glVertex3f(hex_size.first / 2, 0.0f, -hex_size.second / 4);
	glEnd();
	glPopMatrix();
}

std::pair<float, float> SceneLayout::CalculateHexPosition(int row, int column) const
{
	float x_pos = (row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + static_cast<float> (column) * hex_size.first;
	float z_pos = static_cast<float> (row) * 3.0f / 4.0f * hex_size.second;
	return std::pair<float, float>(x_pos, z_pos);
}

void SceneLayout::DrawColorHex(std::array<GLfloat, 3> color, int row, int column) const
{
	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + static_cast<float> (column) * hex_size.first, 0.0, static_cast<float> (row) * 3.0f / 4.0f * hex_size.second);
	glBegin(GL_POLYGON);
	glColor3fv(color.data());	glVertex3f(0.0f, 0.0f, -hex_size.second / 2);
	glColor3fv(color.data());	glVertex3f(-hex_size.first / 2, 0.0f, -hex_size.second / 4);
	glColor3fv(color.data());	glVertex3f(-hex_size.first / 2, 0.0f, hex_size.second / 4);
	glColor3fv(color.data());	glVertex3f(0.0f, 0.0f, hex_size.second / 2);
	glColor3fv(color.data());	glVertex3f(hex_size.first / 2, 0.0f, hex_size.second / 4);
	glColor3fv(color.data());	glVertex3f(hex_size.first / 2, 0.0f, -hex_size.second / 4);
	glEnd();
	glPopMatrix();
}

void SceneLayout::DrawKingdom(std::map<std::string, PlaceWithLimitedInformation, std::less<>> places, [[maybe_unused]] float time_to_proc)
{
	auto playing_camera_north = Spherical(playing_camera.distance, playing_camera.theta, playing_camera.phi + 0.01f);
	gluLookAt(playing_camera.getX(), playing_camera.getY(), playing_camera.getZ(),
			  0.0, 0.0, 0.0,
			  playing_camera_north.getX(), playing_camera_north.getY(), playing_camera_north.getZ());

		  // draw places
	for(auto place = places.begin(); place != places.end(); ++place)
	{
		(this->*drawing_places[place->first])();
	}
	DrawColorHex({0.49f, 0.78f, 0.31f}, -1, 0);
	DrawColorHex({0.49f, 0.78f, 0.31f}, -1, -1);
	DrawColorHex({0.49f, 0.78f, 0.31f}, -1, 1);
	DrawColorHex({0.49f, 0.78f, 0.31f}, 1, -1);
	DrawColorHex({0.4f, 0.76f, 0.79f}, 0, 1);
	DrawColorHex({0.4f, 0.76f, 0.79f}, 1, 1);
}

void SceneLayout::DrawIdle() const
{
	int logs = 6;
	int row = 0;
	int column = 0;
	DrawColorHex({0.76f, 0.7f, 0.5f}, 0, 0);

	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + static_cast<float> (column) * hex_size.first, 0.025f, static_cast<float> (row) * 3.0f / 4.0f * hex_size.second);

	// wood
	std::array<double, 4> eqn = {0.0, 1.0, 0.0, 0.025};
	glClipPlane(GL_CLIP_PLANE0, eqn.data());
	glEnable(GL_CLIP_PLANE0);

	for(int i = 0; i < logs; ++i)
	{
		glPushMatrix();
		glRotatef(static_cast<float> (i) * 60.0f, 0.0, 1.0, 0.0);
		glColor3f(0.55f, 0.38f, 0.26f);
		gluCylinder(qobj, 0.025, 0.05, 0.2, 15, 5);
		DrawCircle3D(0.0, 0.0, 0.2f, 0.05f, 20);
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

void SceneLayout::DrawLumber() const
{
	unsigned int tmp;
	int row = 0;
	int column = -1;
	DrawColorHex({0.49f, 0.78f, 0.31f}, row, column);

	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	std::array<float, 2> wall_sizes = {hex_size.first / 4, hex_size.first / 6};

	std::array<std::array<float, 2>, 4> walls_offsets = {{	{0.0, wall_sizes[1]},
															{wall_sizes[0], 0.0},
															{0.0, -wall_sizes[1]},
															{-wall_sizes[0], 0.0} }};

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + static_cast<float> (column) * hex_size.first - hex_size.first / 8, 0.0, static_cast<float> (row) * 3.0f / 4.0f * hex_size.second - hex_size.second / 8);
	glRotatef(45.0, 0.0, 1.0, 0.0);

	//walls
	for(unsigned int i = 0; i < 4; ++i)
	{
		tmp = i % 2;
		glPushMatrix();
		glColor3f(0.73f, 0.55f, 0.39f);
		glTranslatef(walls_offsets[i][0], 0.0, walls_offsets[i][1]);

		if(tmp == 1) glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex3f(-wall_sizes[tmp], 0.2f, 0.0f);
		glVertex3f(-wall_sizes[tmp], 0.0, 0.0f);
		glVertex3f(wall_sizes[tmp], 0.0, 0.0f);
		glVertex3f(wall_sizes[tmp], 0.2f, 0.0f);
		if(tmp == 1) glVertex3f(0.0, 0.2f + wall_sizes[1], 0.0f);
		glEnd();

		glPopMatrix();
	}
	// roof
	for(unsigned int i = 0; i < 2; ++i)
	{
		tmp = i % 2;
		glPushMatrix();
		glColor3f(0.55f, 0.38f, 0.26f);
		glTranslatef(0.0, 0.2f, (tmp == 0 ? wall_sizes[1] : -wall_sizes[1]));

		glRotatef((tmp == 0 ? -45.0f : 45.0f), 1.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-wall_sizes[0] - 0.05f, 0.22f, 0.0f);
		glVertex3f(-wall_sizes[0] - 0.05f, -0.05f, 0.0f);
		glVertex3f(wall_sizes[0] + 0.05f, -0.05f, 0.0f);
		glVertex3f(wall_sizes[0] + 0.05f, 0.22f, 0.0f);
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
	glTranslatef(0.0, 0.0, 0.07f);
	glRotatef(-60.0, 0.0, 1.0, 0.0);
	glColor3f(0.75f, 0.75f, 0.75f);
	glBegin(GL_QUADS);
	glVertex3f(-0.03f, 0.0, 0.02f);
	glVertex3f(-0.03f, 0.0, -0.02f);
	glVertex3f(0.03f, 0.0, -0.02f);
	glVertex3f(0.03f, 0.0, 0.02f);
	glEnd();


	glPushMatrix();
	glTranslatef(0.03f, 0.0, -0.02f);
	glColor3f(0.55f, 0.38f, 0.26f);
	gluCylinder(qobj, 0.005, 0.005, 0.15, 15, 5);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void SceneLayout::DrawRig() const
{
	int row = 1;
	int column = 0;
	DrawColorHex({0.4f, 0.76f, 0.79f}, row, column);

	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	float leg_offset = hex_size.second / 10;
	float rig_half_size = leg_offset + 0.05f;

	std::array<std::array<float, 2>, 4> legs_offsets = {{	{leg_offset, leg_offset},
															{leg_offset, -leg_offset},
															{-leg_offset, leg_offset},
															{-leg_offset, -leg_offset} }};
	std::array<std::array<float, 2>, 4> walls_offsets = {{	{0.0, rig_half_size},
															{rig_half_size, 0.0},
															{0.0, -rig_half_size},
															{-rig_half_size, 0.0} }};
	std::array<std::array<float, 2>, 4> windows_offsets = {{{0.0, rig_half_size + 10e-6f},
															{rig_half_size + 10e-6f, 0.0},
															{0.0, -rig_half_size - 10e-6f},
															{-rig_half_size - 10e-6f, 0.0} }};

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + static_cast<float> (column) * hex_size.first, 0.0, static_cast<float> (row) * 3.0f / 4.0f * hex_size.second);

	for(unsigned int i = 0; i < 4; ++i)
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
		glTranslatef(walls_offsets[i][0], 0.15f, walls_offsets[i][1]);
		if(i % 2)
			glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-rig_half_size, 0.1f, 0.0f);
		glVertex3f(-rig_half_size, 0.0, 0.0f);
		glVertex3f(rig_half_size, 0.0, 0.0f);
		glVertex3f(rig_half_size, 0.1f, 0.0f);
		glEnd();
		glPopMatrix();

		// window
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(windows_offsets[i][0], 0.175f, windows_offsets[i][1]);
		if(i % 2)
			glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-rig_half_size / 2, 0.05f, 0.0f);
		glVertex3f(-rig_half_size / 2, 0.0, 0.0f);
		glVertex3f(rig_half_size / 2, 0.0, 0.0f);
		glVertex3f(rig_half_size / 2, 0.05f, 0.0f);
		glEnd();
		glPopMatrix();
	}
	glColor3f(0.41f, 0.41f, 0.41f);
	for(int i = 0; i < 2; ++i)
	{
		// floor / roof 
		glPushMatrix();
		glTranslatef(0.0, 0.15f + static_cast<float> (i) * 0.10f, 0.0);
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
	for(int i = 0; i < 3; ++i)
	{
		glPushMatrix();
		glTranslatef(2 * rig_half_size / 3, 0.25, -rig_half_size / 2 + static_cast<float> (i) * rig_half_size / 2);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(qobj, 0.02, 0.02, 0.05, 15, 5);
		glPopMatrix();
	}

	glPopMatrix();
}


void SceneLayout::DrawCastle(int row, int column) const
{
	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	float mini_tower_offset = hex_size.second / 4;

	std::array<std::array<float, 2>, 4> mini_tower_offsets = {{	{mini_tower_offset, mini_tower_offset},
																{mini_tower_offset, -mini_tower_offset},
																{-mini_tower_offset, mini_tower_offset},
																{-mini_tower_offset, -mini_tower_offset} }};
	std::array<std::array<float, 2>, 4> walls_offsets = {{	{0.0, mini_tower_offset},
															{mini_tower_offset, 0.0},
															{0.0, -mini_tower_offset},
															{-mini_tower_offset, 0.0} }};

	glPushMatrix();
	glTranslatef((row % 2 == 0 ? 0.0f : hex_size.first / 2.0f) + static_cast<float> (column) * hex_size.first, 0.0, static_cast<float> (row) * 3.0f / 4.0f * hex_size.second);

	// main-tower
	glPushMatrix();
	glColor3f(0.41f, 0.41f, 0.41f);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluCylinder(qobj, 0.15, 0.15, 0.4, 15, 5);
	// main-tower's roof
	glColor3f(0.74f, 0.29f, 0.24f);
	glTranslatef(0.0, 0.0, 0.4f);
	gluCylinder(qobj, 0.2, 0.0, 0.2, 15, 5);
	glPopMatrix();

	for(unsigned int i = 0; i < 4; ++i)
	{
		// mini-tower
		glPushMatrix();
		glColor3f(0.82f, 0.82f, 0.82f);
		glTranslatef(mini_tower_offsets[i][0], 0.0, mini_tower_offsets[i][1]);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(qobj, 0.075, 0.075, 0.2, 15, 5);
		// mini-tower's roof
		glColor3f(0.74f, 0.29f, 0.24f);
		glTranslatef(0.0, 0.0, 0.2f);
		gluCylinder(qobj, 0.1, 0.0, 0.1, 15, 5);
		glPopMatrix();

		// wall
		glPushMatrix();
		glColor3f(0.66f, 0.66f, 0.66f);
		glTranslatef(walls_offsets[i][0], 0.0, walls_offsets[i][1]);
		if(i % 2)
			glRotatef(90.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-mini_tower_offset, 0.18f, 0.0f);
		glVertex3f(-mini_tower_offset, 0.0, 0.0f);
		glVertex3f(mini_tower_offset, 0.0, 0.0f);
		glVertex3f(mini_tower_offset, 0.18f, 0.0f);
		glEnd();
		glPopMatrix();
	}

	glPopMatrix();
}