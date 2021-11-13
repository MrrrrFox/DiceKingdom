#pragma once
struct Dice
{
	Dice(unsigned int _faces, unsigned int _damage = 0) : faces(_faces), damage(_damage) {}

	unsigned int faces;
	unsigned int damage;   // 0 = no damage, 3 = max damage before dice is destroyed
};
struct DiceCompare
{
	bool operator()(const Dice& d1, const Dice& d2) const
	{
		if(d1.faces != d2.faces)
			return d1.faces < d2.faces;
		else
			return d1.damage < d2.damage;
	}
};
