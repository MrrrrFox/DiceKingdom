#include "pch.h"
#include "Spherical.h"

float Spherical::getX() const
{
	return distance * cos(phi) * cos(theta);
}

float Spherical::getY() const
{
	return distance * sin(phi);
}

float Spherical::getZ() const
{
	return distance * cos(phi) * sin(theta);
}