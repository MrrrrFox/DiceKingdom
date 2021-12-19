#pragma once
#include "Dice.h"
#include "DiceKingdom.h"

class ImGuiLayout
{
	unsigned int WIDTH = 600, HEIGHT = 400;

	std::string menuText = "PRESS SPACE TO START A NEW GAME";

	float panels_widths[4] = { 30.0f, 80.0f, 50.0f, 80.0f };

	DiceKingdom* DK = nullptr;

public:
	void initImGui(sf::RenderWindow * window, unsigned int width, unsigned int height, DiceKingdom * _DK);
	void terminateImGui();

	void drawMenuInfo();
	void drawMaterialsBar(std::vector<Material *> materials);
	void drawPlacePanel(std::string placeName);
};