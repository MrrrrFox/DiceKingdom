#include "pch.h"
#include "Spherical.h"

float Spherical::getX()
{ 
	return distance * cos(phi) * cos(theta); 
}

float Spherical::getY()
{ 
	return distance * sin(phi);
}

float Spherical::getZ()
{ 
	return distance * cos(phi) * sin(theta); 
}