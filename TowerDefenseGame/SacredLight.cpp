#include "SacredLight.h"

SacredLight::SacredLight()
    : Spell(Color(214, 172, 2, 255))
{
    lifetime = 5.0f;
	spellType = SpellType::SacredLightSpell;
}

float SacredLight::getLifetime() const
{
	return this->lifetime;
}

int SacredLight::getRandomDamage() const
{
	return 1 + (rand() % 5); // 1 à 5
}

int SacredLight::getRandomHeal() const
{
	return 6 + (rand() % 10); // 6 à 15
}