#include "pch.h"
#include "Spherical.h"

float Spherical::getX(float add_phi, float add_theta)
{ 
	return distance * cos(phi+add_phi) * cos(theta+add_theta); 
}

float Spherical::getY(float add_phi)
{ 
	return distance * sin(phi+add_phi);
}

float Spherical::getZ(float add_phi, float add_theta)
{ 
	return distance * cos(phi+add_phi) * sin(theta+add_theta); 
}