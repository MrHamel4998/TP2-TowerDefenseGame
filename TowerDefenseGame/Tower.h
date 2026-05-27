#pragma once
#include <SFML/Audio.hpp>
#include "GameObject.h"
#include "Damageable.h"
#include "Subject.h"
#include "IObserver.h"
#include "Constants.h"

class Tower : virtual public GameObject, public Damageable, public Subject, public IObserver
{
public:
	Tower();
	~Tower();
	virtual bool init() = 0;
	virtual void update(const float deltaTime) = 0;

	static Tower* createKingTower();

	TowersType getType() const;
	void setType(const TowersType type);
	void setAttackSound(const SoundBuffer& sound);
	void setDeathSound(const SoundBuffer& sound);
	void setBuildingSound(const SoundBuffer& sound);
	void getAttackSound();
	void getDeathSound();
	void getBuildingSound();
	int getLifePoints() const;
	void setLifePoints(const int desiredLifePoints);
	void setPrice(const int desiredPrice);
	void heal(const int amount);
	virtual void takeDamage(const int amount);
	bool isDead() const;

	void notify(Subject* subject, EventType eventType) override;
	virtual int getPrice() const;


	float damageTakenMultiplier = 1.0f;
	float plagueTimer = 0.0f;
	float colorEffectTimer = 0.0f;

	void updateSpellEffects(float deltaTime);

private:
	void onHealthChanged() override;
	void onDeath() override;

	int price = 0;
	TowersType type;
	Sound* attackSound = nullptr;
	Sound* towerDeathSound = nullptr;
	Sound* towerBuilding = nullptr;
};