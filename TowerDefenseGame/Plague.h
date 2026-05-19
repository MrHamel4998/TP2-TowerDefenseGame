#pragma once
#include "Spell.h"

class Plague : public Spell
{
public:
    Plague();

    float getLifetime() const override;
    int getRandomDamage() const override;
};