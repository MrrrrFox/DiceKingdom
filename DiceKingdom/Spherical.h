#pragma once

struct Spherical
{
    float distance, theta, phi;
    Spherical(float gdistance, float gtheta, float gphi) : distance(gdistance), theta(gtheta), phi(gphi) { }
    float getX(float add_phi = 0.0f, float add_theta = 0.0f);
    float getY(float add_phi = 0.0f);
    float getZ(float add_phi = 0.0f, float add_theta = 0.0f);
};