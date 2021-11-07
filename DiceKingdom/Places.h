#pragma once
struct Dice
{
	Dice(int _faces) : faces(_faces) {}

	int faces;
	public:
};
struct DiceCompare
{
	bool operator()(const Dice &d1, const Dice &d2) const
	{
		return d1.faces<d2.faces;
	}
};

class Place
{
	std::string name;
	std::map<Dice, int, DiceCompare> m;

	public:
	void add(const Dice d, int n);
	void remove(const Dice d, int n);
	int roll();
};