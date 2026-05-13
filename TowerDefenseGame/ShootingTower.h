#pragma once
#include "Tower.h"

class ShootingTower : public Tower 
{
public:
    ShootingTower();
    ~ShootingTower();

    float getRange() const;
    int getAttackDamage() const;
    void setAttackDamage(const int damage);
    void setFireRate(const float rate);
    void setRange(const float range);

private:
    int attackDamage = 0;
    float fireRate = 0;
    float fireRateTimer = 0;
    float range = 300;
    bool isShooting = false;
};