#include "Spell.h"
#include "IObserver.h"
#include <iostream>

Spell::Spell()
{
}

bool Spell::init()
{
    deactivate();

    affectedCount = 0;

    for (int i = 0; i < MAX_TARGETS; i++)
    {
        affectedTargets[i] = nullptr;
    }

    radius = 400.f;
    lifetime = 5.f;
    timer = 0.f;

    return true;
}

void Spell::cast(const Vector2f& position, GameObject* gameEntities[], int entityCount)
{
	this->position = position;
	this->activate();
	timer = lifetime;
	this->affectedCount = 0;

	findTargets(gameEntities, entityCount);

	notifyCast();
}

SpellType Spell::getSpellType() const
{
    return spellType;
}

void Spell::update(float deltaTime)
{
    if (!this->isActive())
        return;

    timer -= deltaTime;

    if (timer <= 0.f) {
        this->deactivate();
    }
}

float Spell::getRemainingLifetimeRatio() const
{
    if (lifetime <= 0.0f)
    {
        return 0.0f;
    }

    float ratio = timer / lifetime;
    if (ratio < 0.0f)
    {
        ratio = 0.0f;
    }
    if (ratio > 1.0f)
    {
        ratio = 1.0f;
    }

    return ratio;
}

void Spell::updateRuneRotation(GameObject& runeSprite, float deltaTime) const
{
    float remainingLifetimeRatio = getRemainingLifetimeRatio();
    float rotationSpeed = 180.0f + (1.0f - remainingLifetimeRatio) * 540.0f;
    runeSprite.rotate(sf::degrees(rotationSpeed * deltaTime));
}

void Spell::notifyCast()
{
    for (int i = 0; i < affectedCount; i++)
    {
        GameObject* target = affectedTargets[i];
        if (target == nullptr) continue;
        IObserver* observer = dynamic_cast<IObserver*>(target);
        if (observer != nullptr)
        {
            observer->notify(this, EventType::SpellCast);
        }
    }
}

void Spell::findTargets(GameObject* entities[], int entityCount)
{
    for (int i = 0; i < entityCount; i++) {

        GameObject* target = entities[i];

        if (target == nullptr || !target->isActive())
        {
            continue;
        }

        float dx = target->getPosition().x - position.x;

        float dy = target->getPosition().y - position.y;

        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= radius) {

            Damageable* damageable = dynamic_cast<Damageable*>(target);

            if (damageable == nullptr)
            {
                continue;
            }

            affectedTargets[affectedCount] = target;

            affectedCount++;
        }
    }
}

bool Spell::containsTarget(GameObject* target) const
{
    for (int i = 0; i < affectedCount; i++) {

        if (affectedTargets[i] == target)
            return true;
    }

    return false;
}

float Spell::getLifetime() const
{
	return this->lifetime;
}