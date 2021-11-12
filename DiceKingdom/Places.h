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
	void add(const Dice d, int n = 1);
	void remove(const Dice d, int n = 1);
	int roll();
	void change_paint(int n);
};