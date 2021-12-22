#include "pch.h"
#include "Spherical.h"

float Spherical::getX() const noexcept
{
	return distance * cos(phi) * cos(theta);
}

float Spherical::getY() const noexcept
{
	return distance * sin(phi);
}

float Spherical::getZ() const noexcept
{
	return distance * cos(phi) * sin(theta);
}