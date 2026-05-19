#include "Plague.h"

Plague::Plague()
    : Spell()
{
    lifetime = 5.0f;
	spellType = SpellType::PlagueSpell;
}

bool Plague::init()
{
	Spell::init();

	this->color = Color(96, 241, 76, 255);

	runeSprite.setTexture(ContentPipeline::getInstance().getRunesTexture());
	centerSprite.setTexture(ContentPipeline::getInstance().getPlagueTexture());
	runeSprite.activate();
	centerSprite.activate();

	return true;
}

void Plague::cast(const Vector2f& position, GameObject* gameEntities[], int entityCount)
{
	Spell::cast(position, gameEntities, entityCount);
	runeSprite.setPosition(position);
	centerSprite.setPosition(position);
}

void Plague::update(float deltaTime)
{
	Spell::update(deltaTime);

	if (!isActive())
	{
		return;
	}

	runeSprite.rotate(sf::degrees(180.0f * deltaTime / getLifetime()));
		}

void Plague::draw(RenderWindow& renderWindow) const
{
	if (!isActive())
	{
		return;
	}

	renderWindow.draw(runeSprite);
	renderWindow.draw(centerSprite);
}

float Plague::getLifetime() const
{
	return lifetime;
}

int Plague::getRandomDamage() const
{
	return 1 + (rand() % 10); // 1 à 10
}

Color Plague::getEffectColor() const
{
	return this->color;
}