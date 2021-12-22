#pragma once

struct Spherical
{
	float distance;
	float theta;
	float phi;
	Spherical(float _distance, float _theta, float _phi) noexcept : distance(_distance), theta(_theta), phi(_phi) {}
	float getX() const noexcept;
	float getY() const noexcept;
	float getZ() const noexcept;
};