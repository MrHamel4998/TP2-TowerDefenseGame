#include "Spell.h"

Spell::Spell(float lifetime, const Color& color)
    : lifetime(lifetime), color(color)
{
}

void Spell::cast(const Vector2f& position, GameObject* gameEntities[], int entityCount)
{
    this->position = position;
	this->isActive = true;
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
    if (!isActive)
        return;

    timer -= deltaTime;

    if (timer <= 0.f) {
        isActive = false;
    }
}

void Spell::notifyCast()
{
    notifyAllObservers(EventType::SpellCast);
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