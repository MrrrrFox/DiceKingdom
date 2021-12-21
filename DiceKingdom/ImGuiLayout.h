#pragma once
#include "Dice.h"
#include "DiceKingdom.h"

class ImGuiLayout
{
	unsigned int WIDTH = 600;
	unsigned int HEIGHT = 400;

	std::string menuText = "PRESS SPACE TO START A NEW GAME";

	std::array<float, 4> panels_widths = {30.0f, 80.0f, 50.0f, 80.0f};

	DiceKingdom* DK = nullptr;

	public:
	void initImGui(sf::RenderWindow* window, unsigned int width, unsigned int height, DiceKingdom* _DK);
	void terminateImGui() const;

	void drawMenuInfo();
	void drawMaterialsBar(const std::vector<Material*>& materials) const;
	void drawPlacePanel(GameView place_enum);
	void drawDiceRow(DiceWithoutHP dice_type, std::map<DiceWithoutHP, int, DiceCompareWithoutHP>& dices_in_place, std::map<DiceWithoutHP, int, DiceCompareWithoutHP>& dices_in_idle, bool idle, const std::string& placeName, int& btn_id);
};