#pragma once
#include "Dice.h"
#include "Place.h"

class ImGuiLayout
{
	int WIDTH = 600, HEIGHT = 400;

	std::string menuText = "PRESS SPACE TO START A NEW GAME";

	float panels_widths[4] = { 30.0f, 80.0f, 50.0f, 80.0f };
	float panels_padding_y = 3.5f;

public:
	void initImGui(sf::RenderWindow * window, int width, int height);
	void terminateImGui();

	void drawMenuInfo();
	void drawMaterialsBar(std::unordered_map<std::string, int>);
	void drawPlacePanel(Place & place);
};