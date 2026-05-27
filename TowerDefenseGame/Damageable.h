#pragma once
#include "GameObject.h"

class Damageable
{
public:
	virtual ~Damageable() = default;

	void initDamageable(int maxHealth);
	virtual void takeDamage(int damage);
	void heal(int amount);

	int getHealth() const;
	int getMaxHealth() const;
	bool isDead() const;

protected:
	virtual void onHealthChanged();
	virtual void onDeath() = 0;

	void setCurrentHealth(int health);

private:
	void notifyHealthChanged();

	int currentHealth = 0;
	int maxHealth = 0;
};