#pragma once
constexpr auto max_dmg = 3;//maximum damage a dice can have before it is broken

struct DiceWithoutHP
{
	explicit DiceWithoutHP(unsigned int _faces) : faces(_faces) {}

	unsigned int faces;
};

struct Dice
{
	Dice(unsigned int _faces, unsigned int _damage = 0) : dice(DiceWithoutHP(_faces)), damage(_damage) {}
	Dice(DiceWithoutHP _dice, unsigned int _damage = 0) : dice(_dice), damage(_damage) {}

	DiceWithoutHP dice;
	unsigned int damage;   // 0 = no damage, 3 = max damage before dice is destroyed
};

struct DiceCompare
{
	bool operator()(const Dice& d1, const Dice& d2) const
	{
		if(d1.dice.faces != d2.dice.faces)
			return d1.dice.faces < d2.dice.faces;
		else
			return d1.damage < d2.damage;
	}
};

struct DiceCompareWithoutHP
{
	bool operator()(const DiceWithoutHP& d1, const DiceWithoutHP& d2) const
	{
		return d1.faces < d2.faces;
	}
};
