#include "Plague.h"

Plague::Plague()
    : Spell(Color(96, 241, 76, 255))
{
    lifetime = 5.0f;
	spellType = SpellType::PlagueSpell;
}

float Plague::getLifetime() const
{
	return lifetime;
}

int Plague::getRandomDamage() const
{
	return 1 + (rand() % 10); // 1 à 10
}