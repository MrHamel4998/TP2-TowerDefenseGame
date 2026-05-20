#include "SacredLight.h"

SacredLight::SacredLight()
    : Spell()
{
    lifetime = 5.0f;
	spellType = SpellType::SacredLightSpell;
}

bool SacredLight::init()
{
	Spell::init();

	this->color = Color(214, 172, 2, 255);

	runeSprite.setTexture(ContentPipeline::getInstance().getRunesTexture());
	centerSprite.setTexture(ContentPipeline::getInstance().getSacredLightTexture());
	runeSprite.activate();
	centerSprite.activate();
	return true;
}

void SacredLight::cast(const Vector2f& position, GameObject* gameEntities[], int entityCount)
{
	Spell::cast(position, gameEntities, entityCount);
	runeSprite.setPosition(position);
	centerSprite.setPosition(position);
}

void SacredLight::update(float deltaTime)
{
	Spell::update(deltaTime);

	if (!isActive())
	{
		return;
	}

	updateRuneRotation(runeSprite, deltaTime);
}

void SacredLight::draw(RenderWindow& renderWindow) const
{
	if (!isActive())
	{
		return;
	}

	renderWindow.draw(runeSprite);
	renderWindow.draw(centerSprite);
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

Color SacredLight::getEffectColor() const
{
	return this->color;
}