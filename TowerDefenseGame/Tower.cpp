#include "Tower.h"
#include <cmath>
#include "ContentPipeline.h" 
#include "KingTower.h"
#include "Spell.h"

Tower::Tower()
{
}

Tower::~Tower()
{
	if (attackSound != nullptr) delete attackSound;
}

Tower* Tower::createKingTower()
{
	return new KingTower();
}

TowersType Tower::getType() const
{
	return type;
}

void Tower::setAttackSound(const SoundBuffer& sound)
{
	attackSound = new Sound(sound);
	attackSound->setVolume(50);
}

void Tower::getAttackSound()
{
	attackSound->play();
}

void Tower::getDeathSound()
{
	towerDeathSound->play();
}

void Tower::getBuildingSound()
{
	towerBuilding->play();
}

void Tower::setDeathSound(const SoundBuffer& sound)
{
	towerDeathSound = new Sound(sound);
}

void Tower::setBuildingSound(const SoundBuffer& sound)
{
	towerBuilding = new Sound(sound);
}

void Tower::setType(const TowersType type)
{
	this->type = type;
}

int Tower::getLifePoints() const
{
	return getHealth();
}

void Tower::setLifePoints(const int desiredLifePoints)
{
	if (getMaxHealth() <= 0)
	{
		initDamageable(desiredLifePoints);
		return;
	}

	Damageable::setCurrentHealth(desiredLifePoints);
}

void Tower::setPrice(const int desiredPrice)
{
	price = desiredPrice;
}

int Tower::getPrice() const
{
	return price;
}

void Tower::heal(const int amount)
{
	Damageable::heal(amount);
}

void Tower::takeDamage(const int amount)
{
	int effective = amount;
	if (damageTakenMultiplier != 1.0f && amount > 0)
	{
		effective = static_cast<int>(round(amount * damageTakenMultiplier));
		if (effective <= 0) effective = 1;
	}
	Damageable::takeDamage(effective);
}

bool Tower::isDead() const
{
	return Damageable::isDead();
}

void Tower::onHealthChanged()
{
	GameObject::setHealth(getHealth(), getMaxHealth());
}

void Tower::onDeath()
{
	Subject::removeObserver(this);

	damageTakenMultiplier = 1.0f;
	plagueTimer = 0.0f;
	colorEffectTimer = 0.0f;
	GameObject::setColor(Color::White);

	towerDeathSound->play();
	notifyAllObservers(EventType::TowerDeactivated);
	deactivate();
}

void Tower::notify(Subject* subject, EventType eventType)
{

}

void Tower::updateSpellEffects(float deltaTime)
{
	if (colorEffectTimer > 0.0f)
	{
		colorEffectTimer -= deltaTime;
		if (colorEffectTimer <= 0.0f)
		{
			GameObject::setColor(Color::White);
			colorEffectTimer = 0.0f;
		}
	}

	if (plagueTimer > 0.0f)
	{
		plagueTimer -= deltaTime;
		if (plagueTimer <= 0.0f)
		{
			damageTakenMultiplier = 1.0f;
			plagueTimer = 0.0f;
		}
	}
}