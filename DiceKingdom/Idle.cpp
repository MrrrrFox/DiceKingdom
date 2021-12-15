#include "pch.h"
#include "Idle.h"

unsigned int Idle::create_resources()
{
	std::map<Dice, int, DiceCompare> damaged;
	for(auto it = (*get_map()).begin(); it != (*get_map()).end(); it++)
	{
		if(it->first.damage > 0)
		{
			damaged[it->first] += it->second;
		}
	}
	for(auto it = damaged.begin(); it != damaged.end(); it++)
	{
		for(int i = 0; i < it->second; i++)
		{
			if((*get_paint()) == 0)
				return 0;
			unsigned int dmg = it->first.damage;
			while(dmg > 0 && (*get_paint()) > 0)
			{
				(*get_paint())--;
				dmg--;
			}
			remove(it->first);
			add(Dice(it->first.dice, dmg));
		}
	}
	return 0;
}
