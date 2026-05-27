#include "Damageable.h"

void Damageable::initDamageable(int maxHealth)
{
	this->maxHealth = maxHealth;
	currentHealth = maxHealth;
	notifyHealthChanged();
}

void Damageable::takeDamage(int damage)
{
	if (damage <= 0 || isDead())
		return;

	currentHealth -= damage;
	if (currentHealth <= 0)
	{
		currentHealth = 0;
		notifyHealthChanged();
		onDeath();
		return;
	}

	notifyHealthChanged();
}

void Damageable::heal(int amount)
{
	if (amount <= 0 || isDead())
		return;

	currentHealth += amount;
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;

	notifyHealthChanged();
}

int Damageable::getHealth() const
{
	return currentHealth;
}

int Damageable::getMaxHealth() const
{
	return maxHealth;
}

bool Damageable::isDead() const
{
	return currentHealth <= 0;
}

void Damageable::setCurrentHealth(int health)
{
	currentHealth = health;
	if (currentHealth < 0)
	{
		currentHealth = 0;
	}
	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}

	notifyHealthChanged();
}

void Damageable::onHealthChanged()
{
	// A voir
}

void Damageable::notifyHealthChanged()
{
	onHealthChanged();
}
