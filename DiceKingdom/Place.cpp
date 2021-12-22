#include "pch.h"
#include "Place.h"

bool Place::is_empty() const noexcept
{
	return m.empty();
}

int Place::count_dices() noexcept
{
	int result = 0;
	for(auto it = m.begin(); it != m.end(); it++)
		result += it->second;
	return result;
}

void Place::add(const Dice d, unsigned int n)
{
	m[d] += n;
}

void Place::remove(const Dice d, unsigned int n)
{
	auto it = m.find(d);
	if(it == m.end())
	{
		std::cerr << "Warning: tried to remove nonexisted element.\n";
		return;
	}
	it->second -= n;
	if(it->second < 0)
		throw std::invalid_argument("Invalid value in Place::remove");
	if(it->second == 0)
		m.erase(it);
}

unsigned int Place::roll()
{
	unsigned int result = 0;
	std::map<Dice, int, DiceCompare> damaged;
	std::map<Dice, int, DiceCompare> damaged_priority;
	std::random_device rd;
	std::mt19937 gen(rd());

	for(auto it = m.begin(); it != m.end(); it++)
	{
		const std::uniform_int_distribution<unsigned int> dis(1, it->first.dice.faces);
		for(int i = 0; i < it->second; i++)
		{
			unsigned int pips;
			pips = dis(gen);
			result += pips;
			if(pips == 1)
			{
				add_to_map_if_damaged(it->first, damaged_priority, damaged);
			}
		}
	}

	// Change the damage status of dices unless there is enough paint stored
	destroy_dices_if_not_enough_paint(damaged_priority);
	damage_dices_if_not_enough_paint(damaged);
	return result;
}

void Place::destroy_dices_if_not_enough_paint(std::map<Dice, int, DiceCompare>& damaged_priority)
{
	for(auto it = damaged_priority.begin(); it != damaged_priority.end(); it++)
	{
		for(int i = 0; i < it->second; i++)
		{
			if((*paint) > 0)
				(*paint)--;
			else
				remove(it->first, 1);
		}
	}
}

void Place::damage_dices_if_not_enough_paint(std::map<Dice, int, DiceCompare>& damaged)
{
	int dmg = 1;
	if(damage_modifier > 1)
	{
		dmg = static_cast<int> (damage_modifier);
	}
	for(auto it = damaged.begin(); it != damaged.end(); it++)
	{
		for(int i = 0; i < it->second; i++)
		{
			if((*paint) > 0)
				(*paint)--;
			else
			{
				remove(it->first);
				add(Dice(it->first.dice.faces, it->first.damage + dmg));
			}
		}
	}
}

void Place::add_to_map_if_damaged(Dice d, std::map<Dice, int, DiceCompare>& damaged_priority, std::map<Dice, int, DiceCompare>& damaged) const
{
	if(damage_modifier <= 0)
		return;
	unsigned int destroyed = max_dmg;	// dices with damage value lower than this number will not be destroyed, only damaged
	if(damage_modifier < 1)
	{
		float check;
		std::random_device rd;
		std::mt19937 gen(rd());
		const std::uniform_real_distribution<float> dis(0, 1);
		check = dis(gen);
		if(check > damage_modifier)
			return;
	}
	if(damage_modifier > max_dmg)
		destroyed = 0;
	else if(damage_modifier > 1)
		destroyed = max_dmg + 1 - static_cast<unsigned int> (damage_modifier);
	if(d.damage < destroyed)
		damaged[d] += 1;
	else
		damaged_priority[d] += 1;
}

std::map<DiceWithoutHP, int, DiceCompareWithoutHP> Place::return_dice_array()
{
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> return_this;
	for(auto it = m.begin(); it != m.end(); it++)
	{
		return_this[it->first.dice] += it->second;
	}
	return return_this;
}

std::string Place::get_name() const
{
	return name;
}