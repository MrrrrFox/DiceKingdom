#pragma once

struct Spherical
{
    float distance, theta, phi;
    Spherical(float _distance, float _theta, float _phi) : distance(_distance), theta(_theta), phi(_phi) { }
    float getX();
    float getY();
    float getZ();
};