#include "pch.h"
#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow * _window, int _WIDTH, int _HEIGHT)
{
	WIDTH = _WIDTH;
	HEIGHT = _HEIGHT;

	window = _window;
	(*window).setFramerateLimit(60);
	(*window).setVerticalSyncEnabled(true);

	imGuiLayout.initImGui(window, WIDTH, HEIGHT);
	sceneLayout.initScene(WIDTH, HEIGHT);
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
					else if (currentView == GameView::KINGDOM_LUMBER /* || currentView == GameView::KINGDOM_RIG */)
					{
						currentView = GameView::KINGDOM;
					}
				}
				else if (event.key.code == sf::Keyboard::Num1 && currentView == GameView::KINGDOM)
				{
					currentView = GameView::KINGDOM_LUMBER;
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
		sceneLayout.DrawKingdom();
		break;
	default:
		std::cerr << "unknown view to draw: " << (int)currentView << std::endl;
	}
}

void GameManager::DrawImGui()
{
	ImGui::SFML::Update(*window, deltaClock.restart());
	
	switch (currentView)
	{
	case GameView::MENU:
		imGuiLayout.drawMenuInfo();
		break;
	case GameView::MAP:
		//imGuiLayout.drawMaterialsBar(DK.get_materials());
		// draw map
		break;
	case GameView::KINGDOM:
		//imGuiLayout.drawMaterialsBar(DK.get_materials());
		// draw kingdom
		break;
	case GameView::KINGDOM_LUMBER:
		//imGuiLayout.drawMaterialsBar(DK.get_materials());
		//imGuiLayout.drawPlacePanel(DK.get_place(KingdomPlace::LUMBER));
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
	default:
		std::cerr << "unknown view: " << (int)currentView << std::endl;
	}
}

void GameManager::CloseGame()
{
	imGuiLayout.terminateImGui();
}
