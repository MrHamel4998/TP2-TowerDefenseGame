#include "ShootingTower.h"
#include "Spell.h"
#include "SacredLight.h"
#include "Plague.h"
#include "ArcherTower.h"
#include "MageTower.h"
#include "Demon.h"

ShootingTower::ShootingTower()
{
}

ShootingTower::~ShootingTower()
{
}

ShootingTower* ShootingTower::create(const TowersType type)
{
	switch (type)
	{
	case TowersType::ARCHER:
		return new ArcherTower();

	case TowersType::MAGE:
		return new MageTower();
	}

	return nullptr;
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

void ShootingTower::setProjectileType(const ProjectileType type)
{
	projectileType = type;
}

void ShootingTower::notify(Subject* subject, EventType eventType)
{
    Spell* spell = dynamic_cast<Spell*>(subject);

    if (eventType != EventType::SpellCast || spell == nullptr || !spell->containsTarget(this))
    {
        return;
    }

    switch (spell->getSpellType())
    {
        case SpellType::SacredLightSpell:
        {
            SacredLight* sacredLight = dynamic_cast<SacredLight*>(spell);

            if (sacredLight == nullptr)
            {
                return;
            }

            fireRateMultiplier = 2.0f;
            fireRateTimer = 5.0f;

            heal(sacredLight->getRandomHeal());

            break;
        }

        case SpellType::PlagueSpell:
        {
            Plague* plague = dynamic_cast<Plague*>(spell);

            if (plague == nullptr)
            {
                return;
            }

            takeDamage(plague->getRandomDamage());

            doubleDamage = true;
            doubleDamageTimer = 5.0f;

            break;
        }
    }
}

void ShootingTower::shoot(float deltaTime, Demon* demons[], int demonCount, Projectile* projectiles[], int projectileCount, int waveNumber)
{
	if (isDead())
	{
		return;
	}

	fireRateTimer += deltaTime;
	if (fireRateTimer < fireRate)
	{
		return;
	}

	Demon* targetDemon = nullptr;
	float closestDistanceSquared = range * range;
	Vector2f towerEmplacement = getPosition();

	for (int i = 0; i < demonCount; i++)
	{
		Demon* demon = demons[i];
		if (demon == nullptr || !demon->isActive())
		{
			continue;
		}

		Vector2f offset = demon->getPosition() - towerEmplacement;
		float distanceSquared = offset.x * offset.x + offset.y * offset.y;
		if (distanceSquared <= closestDistanceSquared)
		{
			closestDistanceSquared = distanceSquared;
			targetDemon = demon;
		}
	}

	if (targetDemon == nullptr)
	{
		return;
	}

	Projectile* projectile = nullptr;
	for (int i = 0; i < projectileCount; i++)
	{
		if (projectiles[i] == nullptr)
		{
			projectiles[i] = new Projectile();
			projectile = projectiles[i];
			break;
		}

		if (!projectiles[i]->isActive())
		{
			projectile = projectiles[i];
			break;
		}
	}

	if (projectile == nullptr)
	{
		return;
	}

	projectile->launch(projectileType, getPosition(), targetDemon, waveNumber);
	fireRateTimer = 0.0f;
}