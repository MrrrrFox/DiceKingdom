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
	int* paint = nullptr;	// points to paint in DiceKingdom -> Resources -> Material paint

	protected:
	Place(const Place&) = default;
	Place& operator=(const Place&) = default;
	Place(Place&&) = default;
	Place& operator=(Place&&) = default;

	public:
	Place(const std::string& _name, float _dmg_modifier = 1) : name(_name), damage_modifier(_dmg_modifier) {}
	virtual ~Place() = default;

	std::map<Dice, int, DiceCompare>* get_map() noexcept
	{
		return &m;
	}
	void set_paint(int* _paint) noexcept
	{
		paint = _paint;
	}
	int* get_paint() noexcept
	{
		return paint;
	}
	bool is_empty() const noexcept;
	int count_dices() const noexcept;
	void add(const Dice d, unsigned int n = 1);
	void remove(const Dice d, unsigned int n = 1);
	unsigned int roll();
	void destroy_dices_if_not_enough_paint(std::map<Dice, int, DiceCompare>& damaged_priority);
	void damage_dices_if_not_enough_paint(std::map<Dice, int, DiceCompare>& damaged);
	void add_to_map_if_damaged(Dice d, std::map<Dice, int, DiceCompare>& damaged_priority, std::map<Dice, int, DiceCompare>& damaged) const;
	std::string get_name() const;
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> return_dice_array();

	virtual unsigned int create_resources() = 0;
};