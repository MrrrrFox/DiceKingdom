#include "pch.h"
#include "Place.h"

bool Place::is_empty() const
{
	return m.empty();
}

int Place::count_dices()
{
	int result = 0;
	for(auto it = m.begin(); it != m.end(); it++)
		result += it->second;
	return result;
}

void Place::add(const Dice d, unsigned int n)
{
	m[d] += n;
	// TODO: Change the number of dices displayed
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
	// TODO: Change the number of dices displayed
	if(it->second < 0)
		throw std::invalid_argument("Invalid value in Place::remove");
	if(it->second == 0)
		m.erase(it);
	// TODO: Stop displaying dices
}

unsigned int Place::roll()
{
	unsigned int result = 0;
	std::map<Dice, int, DiceCompare> damaged;
	std::map<Dice, int, DiceCompare> damaged_priority;

	for(auto it = m.begin(); it != m.end(); it++)
	{
		for(int i = 0; i < it->second; i++)
		{
			unsigned int pips;
			pips = 1 + std::rand() / ((RAND_MAX + 1u) / it->first.dice.faces);
			result += pips;
			if(pips == 1 && damage_modifier > 0)
			{
				unsigned int destroyed = max_dmg;   // dices with damage value lower than this number will not be destroyed, only damaged
				if(damage_modifier < 1)
				{
					float check;
					check = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					if(check > damage_modifier)
						continue;
				}
				if(damage_modifier > max_dmg)
					destroyed = 0;
				else if(damage_modifier > 1)
					destroyed = max_dmg + 1 - static_cast<unsigned int> (damage_modifier);
				if(it->first.damage < destroyed)
					damaged[it->first] += 1;
				else
					damaged_priority[it->first] += 1;
			}
		}
	}
	// TODO: Display result or every single roll, depending on number of dices in Place

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

std::map<DiceWithoutHP, int, DiceCompareWithoutHP> Place::return_dice_array()
{
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> return_this;
	for(auto it = m.begin(); it != m.end(); it++)
	{
		return_this[it->first.dice] += it->second;
	}
	return return_this;
}

std::string Place::get_name()
{
	return name;
}