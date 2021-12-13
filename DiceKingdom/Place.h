#pragma once
#include "Dice.h"

class Place
{
	std::string name;
	std::map<Dice, int, DiceCompare> m;
	float damage_modifier;
	   // =1 -> default
	   // >1 -> increased damage, must be an int
	   // (0,1) -> lower probability to do damage
	   // 0 -> no damage
	   // <0 -> undefined
	int paint;   // nonnegative

	public:
	Place(std::string _name, float _dmg_modifier = 1) : name(_name), damage_modifier(_dmg_modifier), paint(0) {}
	bool is_empty();
	int count_dices();
	// int count_dices(PARAMETERS);
	void add(const Dice d, int n = 1);
	void remove(const Dice d, int n = 1);
	int roll();
	void change_paint(int n);
	std::string get_name();

	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> return_dice_array();

	virtual int create_resources() = 0;
};