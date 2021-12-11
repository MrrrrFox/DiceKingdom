#include "pch.h"
#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow * _window, int _WIDTH, int _HEIGHT)
{
	currentView = GameView::MENU;
	isRunning = true;
	isPlaying = false;
	deltaTime = sf::seconds(0.0f);
	WIDTH = _WIDTH;
	HEIGHT = _HEIGHT;

	world_map = { {WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,		WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::SEA,				WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::DICE_KINGDOM,		WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::SEA,				WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,		WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR} };

	window = _window;

	(*window).setFramerateLimit(60);

	ImGui::SFML::Init(*window);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);



	(*window).setVerticalSyncEnabled(true);

	reshapeScreen((*window).getSize());
	initOpenGL();
}

void GameManager::Run()
{
	while (isRunning)
	{
		sf::Event event;

		while ((*window).pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
			{
				isRunning = false;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					if (currentView == GameView::MENU)
					{
						currentView = GameView::KINGDOM;
						isPlaying = true;
					}
					else if (currentView == GameView::MAP)
					{
						currentView = GameView::KINGDOM;
					}
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					if (currentView == GameView::MENU)
					{
						isRunning = false;
					}
					else if (currentView == GameView::MAP)
					{
						currentView = GameView::MENU;
						isPlaying = false;
					}
					else if (currentView == GameView::KINGDOM)
					{
						currentView = GameView::MAP;
					}
					else if (currentView == GameView::KINGDOM_LUMBER || currentView == GameView::KINGDOM_RIG)
					{
						currentView = GameView::KINGDOM;
					}
				}
				else if (event.key.code == sf::Keyboard::Num1 && currentView == GameView::KINGDOM)
				{
					currentView = GameView::KINGDOM_LUMBER;
				}
				else if (event.key.code == sf::Keyboard::Num2 && currentView == GameView::KINGDOM)
				{
					currentView = GameView::KINGDOM_RIG;
				}
				// handle arrows to decrease / increase dices in places
			}
			
		}

		if (!isRunning)
			break;

		deltaTime += deltaClock.getElapsedTime();

		DrawScene();
		DrawImGui();

		if (isPlaying)
		{
			if (deltaTime > sf::seconds(1.0f))
			{
				Proc();
				deltaTime = sf::seconds(0.0f);
			}
		}

		(*window).display();
	}
}

void GameManager::DrawScene()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

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
		std::cerr << "unknown view to draw";
	}
}

void GameManager::DrawImGui()
{
	ImGui::SFML::Update(*window, deltaClock.restart());
	int w = 5, s=10;
	static int showMaterial = 0;
	std::string text1, text2;
	std::string texts[2] = { "-", "+" };
	float cw;
	
	switch (currentView)
	{
	case GameView::KINGDOM_LUMBER:
		ImGui::SetNextWindowPos(ImVec2(WIDTH/4, HEIGHT/4));
		ImGui::SetNextWindowSize(ImVec2(WIDTH/2, HEIGHT/2));
		ImGui::Begin("Lumber Camp", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
			if (ImGui::Button("Wood"))
				showMaterial = 0;
			ImGui::SameLine();
			if (ImGui::Button("Stone"))
				showMaterial = 1;
			text1 = "Lumber";
			text2 = "Idle";
			cw = ImGui::GetColumnWidth();

			switch (showMaterial)
			{
			case 0:
				ImGui::Text("Lumber");

				ImGui::SameLine(ImGui::GetColumnWidth() - ImGui::CalcTextSize(text2.c_str()).x + 8);
				ImGui::Text("Idle");

				ImGui::PushItemWidth(cw);
				ImGui::SliderInt("", &w, 0, 10);
				//for (int i = 0; i < 2; ++i)
				//{
				//	//ImGui::BeginChild("Haha");
				//	if (ImGui::Button(texts[i].data()))
				//	{
				//		std::cout << i << std::endl;
				//	}
				//	ImGui::SameLine();
				//	ImGui::Text("A");
				//	ImGui::SameLine();
				//	if (ImGui::Button(texts[i].data()))
				//	{
				//		std::cout << i << std::endl;
				//	}
				//	//ImGui::EndChild();
				//}
						
				break;
			case 1:
				ImGui::Text("Lumber");

				ImGui::SameLine(ImGui::GetColumnWidth() - ImGui::CalcTextSize(text2.c_str()).x + 8);
				ImGui::Text("Idle");

				ImGui::PushItemWidth(cw);
				ImGui::SliderInt("", &s, 0, 20);
				break;
			default:
				std::cerr << "unknown material: " << showMaterial << std::endl;
			}
		ImGui::End();
		break;
	case GameView::KINGDOM_RIG:
		ImGui::SetNextWindowPos(ImVec2(WIDTH / 4, HEIGHT / 4));
		ImGui::SetNextWindowSize(ImVec2(WIDTH / 2, HEIGHT / 2));
		ImGui::Begin("Rig Platform", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
		if (ImGui::Button("Wood"))
			showMaterial = 0;
		ImGui::SameLine();
		if (ImGui::Button("Stone"))
			showMaterial = 1;
		text1 = "Rig";
		text2 = "Idle";
		cw = ImGui::GetColumnWidth();

		switch (showMaterial)
		{
		case 0:
			ImGui::Text("Rig");

			ImGui::SameLine(ImGui::GetColumnWidth() - ImGui::CalcTextSize(text2.c_str()).x + 8);
			ImGui::Text("Idle");

			ImGui::PushItemWidth(cw);
			ImGui::SliderInt("", &w, 0, 10);
			break;
		case 1:
			ImGui::Text("Rig");

			ImGui::SameLine(ImGui::GetColumnWidth() - ImGui::CalcTextSize(text2.c_str()).x + 8);
			ImGui::Text("Idle");

			ImGui::PushItemWidth(cw);
			ImGui::SliderInt("", &s, 0, 20);
			break;
		default:
			std::cerr << "unknown material: " << showMaterial << std::endl;
		}
		ImGui::End();
		break;
	}

	ImGui::SFML::Render(*window);
}

void GameManager::Proc()
{
	switch (currentView)
	{
	case GameView::MENU:
		std::cout << "MENU" << std::endl;
		break;
	case GameView::MAP:
		std::cout << "MAP" << std::endl;
		break;
	case GameView::KINGDOM:
		std::cout << "KINGDOM" << std::endl;
		break;
	case GameView::KINGDOM_LUMBER:
		std::cout << "LUMBER" << std::endl;
		break;
	case GameView::KINGDOM_RIG:
		std::cout << "RIG" << std::endl;
		break;
	default:
		std::cerr << "unknown view";
	}
}

void GameManager::CloseGame()
{
	DK.clear();
	ImGui::SFML::Shutdown();
}

void GameManager::initOpenGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

	GLfloat light_ambient_global[4] = { 0.5,0.5,0.5,1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient_global);
}

void GameManager::reshapeScreen(sf::Vector2u size)
{
	glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0f, (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
	glOrtho(-1.245 * ((GLdouble)size.x / (GLdouble)size.y), 1.245 * ((GLdouble)size.x / (GLdouble)size.y), -1.245, 1.245, -3.0, 12.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}