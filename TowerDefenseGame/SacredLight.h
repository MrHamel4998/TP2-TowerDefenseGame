#pragma once
#include "Spell.h"

class SacredLight : public Spell
{
public:
	SacredLight();
	bool init();

	void cast(const Vector2f& position, GameObject* gameEntities[], int entityCount);
	void update(float deltaTime);
	void draw(RenderWindow& renderWindow) const override;

	float getLifetime() const override;
	int getRandomDamage() const override;
	int getRandomHeal() const;
	Color getEffectColor() const override;

private:
	GameObject runeSprite;
	GameObject centerSprite;
};
