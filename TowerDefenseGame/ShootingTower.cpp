#include "ShootingTower.h"
#include "Spell.h"
#include "SacredLight.h"
#include "Plague.h"

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