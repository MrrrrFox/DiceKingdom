#pragma once

struct Spherical
{
	float distance;
	float theta;
	float phi;
	Spherical(float _distance, float _theta, float _phi) : distance(_distance), theta(_theta), phi(_phi) {}
	float getX() const;
	float getY() const;
	float getZ() const;
};