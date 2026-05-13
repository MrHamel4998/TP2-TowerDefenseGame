#include "Demon.h"
#include "ContentPipeline.h"

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
	this->health = BASE_HEALTH;
	this->maxHp = BASE_HEALTH;
	this->speed = (0.9f + 0.1f * waveNumber) * 60.0f;
	isDying = false;
	setHealth(health, maxHp);
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

   if (health <= 0)
   {
	   setActiveAnimation(DEATH, true);
	   isDying = true;
	   updateAnimation(deltaTime);
	   return;
   }

   updateAnimation(deltaTime);

   // Le démon a atteint la fin du chemin : déclencher l'animation de mort
   if (currentTargetWaypoint == nullptr)
   {
	   takeDamage(health); // Mettre la santé à 0 pour déclencher l'animation de mort
       return;
   }

	Vector2f currentPos = getPosition();
	Vector2f targetPos = currentTargetWaypoint->getPosition();
	Vector2f direction = targetPos - currentPos;
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance < 2.0f)
	{
		currentTargetWaypoint = currentTargetWaypoint->getNextWaypoint();
		if (currentTargetWaypoint == nullptr)
		{
			return;
		}
		return;
	}

	if (distance > 0.0f)
	{
		direction /= distance;
		float movement = speed * deltaTime;
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

void Demon::takeDamage(int damage)
{
	if (isDying)
		return;

	health -= damage;
	if (health <= 0)
	{
		health = 0;
		setHealth(health, maxHp);
		setActiveAnimation(DEATH, true);
		isDying = true;
		return;
	}

	setHealth(health, maxHp);
}