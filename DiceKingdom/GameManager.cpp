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

	procTime = sf::seconds(1.0f);

	sceneLayout.set_width_height(WIDTH,HEIGHT);

	world_map = { {WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,		WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::SEA,				WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::DICE_KINGDOM,		WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::SEA,			WorldTerrain::SEA,				WorldTerrain::SEA,			WorldTerrain::FOG_OF_WAR},
				  {WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR,		WorldTerrain::FOG_OF_WAR,	WorldTerrain::FOG_OF_WAR} };

	window = _window;

	(*window).setFramerateLimit(60);

	ImGui::SFML::Init(*window);

	ImGuiStyle& style = ImGui::GetStyle();
	/*style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);*/



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
				if (event.key.code == sf::Keyboard::Enter)
				{
					sceneLayout.set_perspective_projection(!sceneLayout.get_perspective_projection());
				}
				else if (event.key.code == sf::Keyboard::Space)
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
			if (deltaTime > procTime)
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
		sceneLayout.DrawMenu();
		break;
	case GameView::MAP:
		sceneLayout.DrawWorldMap(world_map);
		break;
	case GameView::KINGDOM:
	case GameView::KINGDOM_LUMBER:
	case GameView::KINGDOM_RIG:
		//sceneLayout.DrawKingdom(DK.get_places(), currentView);
		break;
	default:
		std::cerr << "unknown view to draw: " << (int)currentView << std::endl;
	}
}

void GameManager::DrawImGui()
{
	ImGui::SFML::Update(*window, deltaClock.restart());
	int btn_id = 0;
	int tab[3] = { 5, 10, 30 };
	float padding_y = 3.5f;
	float col_widths[4] = {30.0f, 80.0f, 50.0f, 80.0f};
	ImVec2 c_pos;
	std::string texts[3] = { "D4", "D6", "D20" };
	ImVec4 color = { 0.0f, 0.0f, 0.0f, 0.7f };
	std::string menuText = "PRESS SPACE TO START A NEW GAME";
	
	switch (currentView)
	{
	case GameView::MENU:
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, color);

		ImGui::SetNextWindowPos(ImVec2(0.3f * WIDTH, 0.9f * HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(0.4f * WIDTH, 20.0f));
		ImGui::Begin("Title Bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(menuText.data()).x) * 0.5f);
			ImGui::Text(menuText.data());
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		break;
	case GameView::KINGDOM_LUMBER:
		ImGui::SetNextWindowPos(ImVec2(WIDTH/4.0f, HEIGHT/4.0f));
		ImGui::SetNextWindowSize(ImVec2(WIDTH/2.0f, HEIGHT/2.0f));
		ImGui::Begin("Lumber Camp", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

		if (ImGui::BeginTable("Lumber Camp", 4, ImGuiTableFlags_BordersInnerH))
		{
			ImGui::TableSetupColumn("dice type", ImGuiTableColumnFlags_WidthFixed, col_widths[0]);
			ImGui::TableSetupColumn("decreasing values", ImGuiTableColumnFlags_WidthFixed, col_widths[1]);
			ImGui::TableSetupColumn("quantity", ImGuiTableColumnFlags_WidthFixed, col_widths[2]);
			ImGui::TableSetupColumn("increasing values", ImGuiTableColumnFlags_WidthFixed, col_widths[3]);

			for (int row = 0; row < 3; ++row)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				c_pos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(c_pos.x + (col_widths[0] - ImGui::CalcTextSize(texts[row].data()).x) / 2.0f, c_pos.y+padding_y));
				ImGui::Text(texts[row].data());

				ImGui::TableSetColumnIndex(1);
				c_pos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(c_pos.x + col_widths[1] - 15.0f, c_pos.y));
				ImGui::PushID(btn_id++);
				if (ImGui::Button("-"))
					std::cout << "-" + texts[row] << std::endl;
				ImGui::PopID();
					

				ImGui::TableSetColumnIndex(2);
				c_pos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(c_pos.x + (col_widths[2] - ImGui::CalcTextSize(std::to_string(tab[row]).data()).x) / 2.0f, c_pos.y));
				ImGui::Text(std::to_string(tab[row]).data());

				ImGui::TableSetColumnIndex(3);
				ImGui::PushID(btn_id++);
				if (ImGui::Button("+"))
					std::cout << "+" + texts[row] << std::endl;
				ImGui::PopID();
			}

			ImGui::EndTable();
		}
		ImGui::End();
		break;
	case GameView::KINGDOM_RIG:
		std::cout << "SHOWING RIG PANEL" << std::endl;
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
	//DK.clear();
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
	if (sceneLayout.get_perspective_projection()) gluPerspective(45.0f, (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
	else glOrtho(-((GLdouble)size.x / (GLdouble)size.y), ((GLdouble)size.x / (GLdouble)size.y), -1.0, 1.0, -3.0, 12.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}