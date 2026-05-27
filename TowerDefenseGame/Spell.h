#pragma once
#include "Subject.h"
#include "GameObject.h"
#include "Damageable.h"
#include "Constants.h"
#include "ContentPipeline.h"
using namespace sf;

class Spell : public GameObject, public Subject
{
public:
    Spell();
    virtual ~Spell() = default;

    bool init();

    void cast(const Vector2f& position, GameObject* gameEntities[], int entityCount);

	bool containsTarget(GameObject* target) const;

    void update(float deltaTime);

	void findTargets(GameObject* entities[], int entityCount);

	SpellType getSpellType() const;

	virtual int getRandomDamage() const = 0;
	virtual float getLifetime() const = 0;
	virtual Color getEffectColor() const = 0;

protected:
	void updateRuneRotation(GameObject& runeSprite, float deltaTime) const;

	static const int MAX_TARGETS = 20;
	SpellType spellType;

	GameObject* affectedTargets[MAX_TARGETS];
    int affectedCount = 0;

    void notifyCast();

    Vector2f position;
    Color color;
    float radius = 400.0f;
    float lifetime;
    float timer = 0.0f;

    float getRemainingLifetimeRatio() const;
};