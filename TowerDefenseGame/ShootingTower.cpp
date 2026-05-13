#include "ShootingTower.h"

ShootingTower::ShootingTower()
{
}

ShootingTower::~ShootingTower()
{
}

float ShootingTower::getRange() const
{
    return range;
}

int ShootingTower::getAttackDamage() const
{
    return attackDamage;
}

void ShootingTower::setAttackDamage(const int damage)
{
    attackDamage = damage;
}

void ShootingTower::setFireRate(const float rate)
{
    fireRate = rate;
}

void ShootingTower::setRange(const float range)
{
    this->range = range;
}