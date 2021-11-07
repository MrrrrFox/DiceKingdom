#include "pch.h"
#include "Places.h"

void Place::add(const Dice d, int n)
{
	m[d]+=n;
}

void Place::remove(const Dice d, int n)
{
	auto it=m.find(d);
	it->second-=n;
	if(it->second==0)
		m.erase(it);
	if(it->second<0)
		throw std::runtime_error("Invalid value");
}

int Place::roll()
{
	int result=0;
	for(auto it=m.begin(); it!=m.end(); it++)
	{
		for(int i=0; i<it->second; i++)
			result+=1+std::rand()/((RAND_MAX+1u)/it->first.faces);
	}
	return result;
}
