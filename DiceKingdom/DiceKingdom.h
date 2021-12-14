#pragma once
#include "Lumber.h"
#include "enums.h"

class DiceKingdom
{
	Lumber lumber;

	std::unordered_map<std::string, int> materials;

public:
	DiceKingdom();

	Place& get_place(Places place);
	std::unordered_map<std::string, int> get_materials();
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> get_dices_from(Places place);
};