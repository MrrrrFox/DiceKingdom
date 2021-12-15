#include "pch.h"
#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow* _window, int _WIDTH, int _HEIGHT)
{
	WIDTH = _WIDTH;
	HEIGHT = _HEIGHT;

	window = _window;
	(*window).setFramerateLimit(60);
	(*window).setVerticalSyncEnabled(true);

	imGuiLayout.initImGui(window, WIDTH, HEIGHT, &DK);
	sceneLayout.initScene(std::pair<int,int>(WIDTH, HEIGHT), get_DiceKingdom_position());
	//sceneLayout.initScene(std::pair<int, int>(WIDTH, HEIGHT), std::pair<int,int>{0,0});
}

void GameManager::Run()
{
	while(isRunning)
	{
		sf::Event event;

		while((*window).pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if(event.type == sf::Event::Closed)
			{
				isRunning = false;
			}
			if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Enter)
				{
					sceneLayout.set_perspective_projection(!sceneLayout.get_perspective_projection());
					sceneLayout.reshapeScreen((*window).getSize());
				}
				else if(event.key.code == sf::Keyboard::Space)
				{
					if(currentView == GameView::MENU)
					{
						currentView = GameView::KINGDOM;
						isPlaying = true;
						sceneLayout.set_perspective_projection(!sceneLayout.get_perspective_projection());
						sceneLayout.reshapeScreen((*window).getSize());
					}
					else if(currentView == GameView::MAP)
					{
						currentView = GameView::KINGDOM;
					}
				}
				else if(event.key.code == sf::Keyboard::Escape)
				{
					if(currentView == GameView::MENU)
					{
						isRunning = false;
					}
					else if(currentView == GameView::MAP)
					{
						currentView = GameView::MENU;
						isPlaying = false;
						sceneLayout.set_perspective_projection(!sceneLayout.get_perspective_projection());
						sceneLayout.reshapeScreen((*window).getSize());
					}
					else if(currentView == GameView::KINGDOM)
					{
						currentView = GameView::MAP;
					}
					else if(currentView == GameView::KINGDOM_LUMBER /* || currentView == GameView::KINGDOM_RIG */)
					{
						currentView = GameView::KINGDOM;
					}
				}
				else if(event.key.code == sf::Keyboard::Num1 && currentView == GameView::KINGDOM)
				{
					currentView = GameView::KINGDOM_LUMBER;
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					sceneLayout.map_camera.theta -= (float)fmod(3 * deltaTime.asSeconds(), M_PI);
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					sceneLayout.map_camera.theta += (float)fmod(3 * deltaTime.asSeconds(), M_PI);
				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta < 0 && sceneLayout.map_camera.distance < 10.0f)
				{
					sceneLayout.map_camera.distance += 10 * deltaTime.asSeconds();
				}
				else if (event.mouseWheelScroll.delta > 0 && sceneLayout.map_camera.distance > 1.5f)
				{
					sceneLayout.map_camera.distance -= 10 * deltaTime.asSeconds();
				}
			}
		}

		if(!isRunning)
			break;

		deltaTime = deltaClock.getElapsedTime();
		procCountUpTime += deltaTime;

		DrawScene();
		DrawImGui();

		if(isPlaying)
		{
			if (procCountUpTime > procTime)
			{
				Proc();
				procCountUpTime = sf::seconds(0.0f);
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

	switch(currentView)
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
			std::cerr << "unknown view to draw: " << (int) currentView << std::endl;
	}
}

void GameManager::DrawImGui()
{
	ImGui::SFML::Update(*window, deltaClock.restart());

	switch(currentView)
	{
	case GameView::MENU:
		imGuiLayout.drawMenuInfo();
		break;
	case GameView::MAP:
		imGuiLayout.drawMaterialsBar(DK.get_resources());
		break;
	case GameView::KINGDOM:
		imGuiLayout.drawMaterialsBar(DK.get_resources());
		break;
	case GameView::KINGDOM_LUMBER:
		imGuiLayout.drawMaterialsBar(DK.get_resources());
		imGuiLayout.drawPlacePanel("Lumber Camp");
		break;
	}

	ImGui::SFML::Render(*window);
}

void GameManager::Proc()
{
	switch(currentView)
	{
		case GameView::MENU:
			std::cout << "MENU" << std::endl;
			break;
		case GameView::MAP:
			std::cout << "MAP" << std::endl;
			break;
		case GameView::KINGDOM:
			//std::cout << "KINGDOM" << std::endl;
		case GameView::KINGDOM_LUMBER:
			//std::cout << "LUMBER" << std::endl;
			DK.create_resources();
			break;
		default:
			std::cerr << "unknown view: " << (int) currentView << std::endl;
	}
}

void GameManager::CloseGame()
{
	imGuiLayout.terminateImGui();
}

std::pair<int, int> GameManager::get_DiceKingdom_position()
{
	for (int i=0; i<world_map.size(); ++i)
	{
		for (int j=0; j<world_map[i].size(); ++j)
		{
			if (world_map[i][j].terrainType == TerrainType::DICE_KINGDOM)
				return std::pair<int, int>(i, j);
		}
	}
	std::cerr << "dice kingdom not found on map" << std::endl;
	return std::pair<int, int>(-1, -1);
}
