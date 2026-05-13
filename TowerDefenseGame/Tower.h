#pragma once
#include <SFML/Audio.hpp>
#include "GameObject.h"
#include "Damageable.h"
#include "Subject.h"
#include "TowersType.h"
#include "TowerEmplacement.h"
#include "EventType.h"
/*
Metrics des tours (à effacer à la fin)

- Rayon du cercle de collision: taille en X du sprite ou du rectangle d'animation divisée par 4
- Points de vie: Tour du roi: 500, autres: 250.
- Cadence de tir: tour d'archer: 0.8 secondes entre les tirs (48 rafraichissements d'écran à 60 fps), tour de mage, 1.5 seconde et demie (90 rafraichissements)
- Portée de tir: 300 chacune

- Rectangle d'animation de la tour du mage: 150 X 250
- Vitesse d'animation de la tour du mage: 0.15 Secs
- L'image 0 est l'image idle. Quand on initie le tir, on passe tout de suite à l'image 1.
*/

class Tower : public GameObject, public Damageable, public Subject
{
public:
	Tower();
	~Tower();
	virtual bool init() = 0;
	virtual void update(const float deltaTime) = 0;

	static Tower* create(const TowersType type);

	TowersType getType() const;
	void setType(const TowersType type);
	int getLifePoints() const;
	void setLifePoints(const int desiredLifePoints);
	void heal(const int amount);
	void takeDamage(const int amount);
	bool isDead() const;
	TowerEmplacement getEmplacement() const;

private:
	void onHealthChanged() override;
	void onDeath() override;

	TowersType type;
	TowerEmplacement emplacement;
	Sound* towerSound = nullptr;
};
