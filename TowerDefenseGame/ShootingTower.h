#pragma once
#include "Tower.h"
#include "Projectile.h"

class Demon;

class ShootingTower : public Tower 
{
public:
    ShootingTower();
    ~ShootingTower();

    static ShootingTower* create(const TowersType type);

    float getRange() const;
    int getAttackDamage() const;
    void setAttackDamage(const int damage);
    void setFireRate(const float rate);
    void setRange(const float range);
    bool getShootingStatus();
    void setProjectileType(const ProjectileType type);
    void notify(Subject* subject, EventType eventType) override;
    void shoot(float deltaTime, Demon* demons[], int towerCount, Projectile* projectiles[], int projectileCount, int waveNumber);
    virtual void onShoot() {}

private:

    int attackDamage = 0;
    float fireRate = 0;
    float fireRateTimer = 0;
    float range = 300;
    bool isShooting = false;

    float fireRateMultiplier = 1.f;

    bool doubleDamage = false;
    float doubleDamageTimer = 0.f;
    ProjectileType projectileType;
};