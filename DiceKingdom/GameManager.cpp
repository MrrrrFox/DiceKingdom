#include "pch.h"
#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow* _window, unsigned int _WIDTH, unsigned int _HEIGHT) : WIDTH(_WIDTH), HEIGHT(_HEIGHT), window(_window)
{
	(*window).setFramerateLimit(60);
	(*window).setVerticalSyncEnabled(true);

	imGuiLayout.initImGui(window, WIDTH, HEIGHT, &DK);
	sceneLayout.initScene(std::pair<unsigned int, unsigned int>(WIDTH, HEIGHT));
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
				KeyboardEvent(event);
			}
			else if(event.type == sf::Event::MouseWheelScrolled)
			{
				MouseEvent(event);
			}
		}

		if(!isRunning)
			break;

		deltaTime = deltaClock.getElapsedTime();
		procCountUpTime += deltaTime;

		DrawScene();
		DrawImGui();

		if(isPlaying && procCountUpTime > procTime)
		{
			Proc();
			procCountUpTime = sf::seconds(0.0f);
		}
		(*window).display();
	}
}

void GameManager::KeyboardEvent(sf::Event event)
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
		else if(currentView == GameView::KINGDOM_IDLE
				|| currentView == GameView::KINGDOM_LUMBER
				|| currentView == GameView::KINGDOM_RIG)
		{
			currentView = GameView::KINGDOM;
		}
	}
	else if(currentView == GameView::KINGDOM)
	{
		if(event.key.code == sf::Keyboard::Num1)
		{
			currentView = GameView::KINGDOM_IDLE;
		}
		else if(event.key.code == sf::Keyboard::Num2)
		{
			currentView = GameView::KINGDOM_LUMBER;
		}
		else if(event.key.code == sf::Keyboard::Num3)
		{
			currentView = GameView::KINGDOM_RIG;
		}
	}
	if(currentView == GameView::MAP || currentView == GameView::KINGDOM)
	{
		if(event.key.code == sf::Keyboard::Right)
		{
			sceneLayout.playing_camera.theta -= (float) fmod(3 * deltaTime.asSeconds(), M_PI);
		}
		else if(event.key.code == sf::Keyboard::Left)
		{
			sceneLayout.playing_camera.theta += (float) fmod(3 * deltaTime.asSeconds(), M_PI);
		}
		else if(event.key.code == sf::Keyboard::Down /* && sceneLayout.playing_camera.phi > M_PI / 5.0f */)
		{
			sceneLayout.playing_camera.phi -= deltaTime.asSeconds();
		}
		else if(event.key.code == sf::Keyboard::Up && sceneLayout.playing_camera.phi < M_PI / 2.0f)
		{
			sceneLayout.playing_camera.phi += deltaTime.asSeconds();
		}
	}
}

void GameManager::MouseEvent(sf::Event event)
{
	if(event.mouseWheelScroll.delta < 0 && sceneLayout.playing_camera.distance < 10.0f)
	{
		sceneLayout.playing_camera.distance += 10 * deltaTime.asSeconds();
	}
	else if(event.mouseWheelScroll.delta > 0 && sceneLayout.playing_camera.distance > 2.0f)
	{
		sceneLayout.playing_camera.distance -= 10 * deltaTime.asSeconds();
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
		case GameView::KINGDOM_IDLE:
		case GameView::KINGDOM_LUMBER:
		case GameView::KINGDOM_RIG:
			sceneLayout.DrawKingdom(DK.get_map_of_places_with_limited_information(), (sf::seconds(1.0f) - procCountUpTime) / procTime);
			break;
		default:
			throw std::invalid_argument("unknown view to draw: " + std::to_string((int) currentView));
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
		case GameView::KINGDOM:
			imGuiLayout.drawMaterialsBar(DK.get_resources());
			break;
		case GameView::KINGDOM_IDLE:
		case GameView::KINGDOM_LUMBER:
		case GameView::KINGDOM_RIG:
			imGuiLayout.drawMaterialsBar(DK.get_resources());
			imGuiLayout.drawPlacePanel(currentView);
			break;
		default:
			std::cerr << "Unrecognized value of currentView(" << static_cast<int> (currentView) << " in GameManager::DrawImGui()\n";
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
		case GameView::KINGDOM:
		case GameView::KINGDOM_IDLE:
		case GameView::KINGDOM_LUMBER:
		case GameView::KINGDOM_RIG:
			DK.create_resources();
			break;
		default:
			throw std::invalid_argument("unknown view to draw: " + std::to_string((int) currentView));
	}
}

void GameManager::CloseGame()
{
	imGuiLayout.terminateImGui();
}