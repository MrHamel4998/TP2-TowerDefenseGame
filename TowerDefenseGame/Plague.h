#pragma once
#include "Spell.h"
using namespace sf;

class Plague : public Spell
{
public:
	Plague();
	bool init();

	void cast(const Vector2f& position, GameObject* gameEntities[], int entityCount);
	void update(float deltaTime);
	void draw(RenderWindow& renderWindow) const override;

	float getLifetime() const override;
	int getRandomDamage() const override;
	Color getEffectColor() const override;

private:
	GameObject runeSprite;
	GameObject centerSprite;
};
