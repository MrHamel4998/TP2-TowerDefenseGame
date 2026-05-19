#include "Demon.h"
#include "ContentPipeline.h"
#include "Tower.h"
#include "Spell.h"
#include "SacredLight.h"
#include "Plague.h"

Demon::Demon()
{
		setTexture(ContentPipeline::getInstance().getDemonTexture());
		init();
}

bool Demon::init()
{
       if (!initAnimationsStructure(ANIMATION_COUNT, Vector2i(RECTANGLE_SIZE_X, RECTANGLE_SIZE_Y))) return false;
	   setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	   setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	   if (!initAnimation(FLY, 5, 0.1f, AnimationType::Circular)) return false;
	   if (!initAnimation(DEATH, 5, 0.1f, AnimationType::Linear)) return false;
	   setActiveAnimation(FLY, true);
	   
	   getHealthBar().initHealthBar(ContentPipeline::getInstance().getRedBarTexture(), ContentPipeline::getInstance().getGreenBarTexture());
	   
	   return true;
}

void Demon::spawn(const Vector2f& position, Waypoint* firstWaypoint, int waveNumber)
{
	setTexture(ContentPipeline::getInstance().getDemonTexture());
	setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	setActiveAnimation(FLY, true);
	setPosition(position);
	this->currentTargetWaypoint = firstWaypoint;
	this->speed = (0.9f + 0.1f * waveNumber) * 60.0f;
	isDying = false;
	plagueTimer = 0.0f;
	initDamageable(BASE_HEALTH);
	setHealth(getHealth(), getMaxHealth());
	activate();
}

void Demon::update(float deltaTime)
{
   if (isDying)
   {
	   updateAnimation(deltaTime);
	   if (isCurrentLinearAnimationIsOver())
	   {
		   deactivate();
	   }
	   return;
   }

   if (isDead())
   {
	   setActiveAnimation(DEATH, true);
	   isDying = true;
	   updateAnimation(deltaTime);
	   return;
   }

   updateAnimation(deltaTime);

   // DEBUG: Les démons perdent 1 PV par seconde
   plagueTimer += deltaTime;
   if (plagueTimer >= 1)
   {
		takeDamage(1);
		plagueTimer = 0.0f;
   }

   // Le démon a atteint la fin du chemin : déclencher l'animation de mort
   if (currentTargetWaypoint == nullptr)
   {
	   takeDamage(getHealth()); // Mettre la santé à 0 pour déclencher l'animation de mort
       return;
   }

	Vector2f currentPos = getPosition();
	Vector2f targetPos = currentTargetWaypoint->getPosition();
	Vector2f direction = targetPos - currentPos;
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance < 2.0f)
	{
		if (currentTargetWaypoint->hasAlternative())
		{
			if (rand() % 2 == 0)
			{
				currentTargetWaypoint = currentTargetWaypoint->getNextWaypoint();
			}
			else
			{
				currentTargetWaypoint = currentTargetWaypoint->getAlternativeWaypoint();
			}
		}
		else
		{
			currentTargetWaypoint = currentTargetWaypoint->getNextWaypoint();
		}

		if (currentTargetWaypoint == nullptr)
		{
			return;
		}
		return;
	}

	if (distance > 0.0f)
	{
		direction /= distance;
		if (sacredLightTimer > 0.0f) {
			sacredLightTimer -= deltaTime;
			if (sacredLightTimer <= 0.0f) {
				sacredLightRatio = 1.0f; 
			}
		}

		float movement = speed * deltaTime * sacredLightRatio;
		move(direction * movement);
	}

	// Flip le démon en fonction de la direction du waypoint
	if (currentTargetWaypoint != nullptr)
	{
		Vector2f currentPos = getPosition();
		Vector2f targetPos = currentTargetWaypoint->getPosition();

		if (targetPos.x > currentPos.x)
			setScale(Vector2f(1.0f, 1.0f));    // Waypoint à droite
		else
			setScale(Vector2f(-1.0f, 1.0f));   // Waypoint à gauche
	}
}

void Demon::notify(Subject* subject, EventType eventType)
{
	Spell* spell = dynamic_cast<Spell*>(subject);

	if (eventType != EventType::SpellCast || spell == nullptr || !spell->containsTarget(this))
	{
		return;
	}

	switch (spell->getSpellType())
	{
	case SpellType::SacredLightSpell:
	{
		SacredLight* sacredLight = dynamic_cast<SacredLight*>(spell);

		if (sacredLight == nullptr)
		{
			return;
		}

		takeDamage(sacredLight->getRandomDamage());

		sacredLightRatio = 0.5f;
		sacredLightTimer = 5.0f;

		break;
	}

	case SpellType::PlagueSpell:
	{
		Plague* plague = dynamic_cast<Plague*>(spell);

		if (plague == nullptr)
		{
			return;
		}

		takeDamage(plague->getRandomDamage());

		plagueDamageMultiplier = 2.0f;
		plagueTimer = 5.0f;

		break;
	}
	}
}

void Demon::onHealthChanged()
{
	Subject::notifyAllObservers(EventType::DemonDamageTaken);
}

void Demon::onDeath()
{
	Subject::notifyAllObservers(EventType::DemonKilled);
}