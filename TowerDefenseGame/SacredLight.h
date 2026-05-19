#pragma once
#include "Spell.h"

class SacredLight : public Spell
{
public:
    SacredLight();

    float getLifetime() const override;
    int getRandomDamage() const override;
    int getRandomHeal() const;
};