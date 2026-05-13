#pragma once
#include "Tower.h"

class ShootingTower : public Tower 
{
public:

private:
    int attackDamage = 0;
    float fireRate = 0.f;
    float fireRateTimer = 0.f;
    float range = 300.f;
    bool isShooting = false;
};