#pragma once
#include "Subject.h"
#include "GameObject.h"
#include "Damageable.h"
#include "Constants.h"
#include "ContentPipeline.h"
using namespace sf;

/*
Metrics du Spell (à effacer à la fin)
- Portée des deux types de Spell: 400
- Durée de vie des deux spells: 5 secondes
- Vitesse de rotation:
    - Va de 0 à 20, progressivement en avancant dans le temps
    - Ajoute 1/15 à la rotation à chaque rafraichissement d'écran.
    - Ajoute deltaTime / 15 à chaque update si on utilise le deltaTime

- Couleur transmise par la peste: Color(96, 241, 76, 255);
- Couleur transmise par la sacred light: Color(214, 172, 2, 255);

- Les spells sont des acteurs très simples qui doivent seulement jouer un petit effet visuel pour
montrer qu’il est actif.  Les effets des spells sont sensés s’être terminé chez les acteurs qu’ils
affectent à la fin de la durée de vie des spells (même si dans les faits les deux durées seront
indépendantes). - L’effet visuel des spells présenté dans l’exemple est une rotation des runes qui accélère jusqu’à
à la fin de vie.  La rune est un attribut sprite ajouté au spell. - Chaque spell ne peut être présent en jeu qu’en un seul exemplaire la fois pour chaque spell. - Un spell est un sujet.  Pour garder les choses simples, vous lancez la notification à son début
d’activation.  La désactivation du spell indique au joueur que son effet devrait être terminé chez
les acteurs. - Un acteur qui n’était pas à portée d’un spell quand il a été lancé mais qui le devient n’est pas
affecté par celui-ci.  L’affectation se fait seulement au notify. - Sacred Light a une portée de 400 et une durée de 5 secondes.  Tous les démons à portée
reçoivent 1 à 5 points de dommage et diminuent leur vitesse de moitié tandis que les tours sont
immédiatement soignées de de 6 à 15 points de vie et doublent leur cadence de tir. - Plague a une portée de 400 et une durée de 5 secondes.  Tous les acteurs qui ont des points de
vie, amis et ennemis, subissent 1 à 10 points de dommage à l’initialisation du sort et par la suite,
tout dommage reçu pendant la durée du sort est doublé (il faut être prudent lors de son
utilisation). - Les tours et les démons sont observateurs, ce sont eux qui doivent de manière interne gérer
leurs états particuliers et le temps que ça les affecte.  Cependant, toutes les infos par rapport au
sort doivent venir du sort.  En ce sens, le 5 secondes de temps de sort pourrait changer si on le
décide, tout comme les autres ratios, mais toute cette info doit absolument venir du sort. - Pour sécuriser le code, vous pouvez ajouter une variable « multiplicateur » pour chaque
variable affectée potentiellement par un spell.  Cette variable vaudrait 1 par défaut, est modifiée
quand affectée par un spell et ramené à 1 à la fin de l’effet.
Par exemple voici la ligne de code qui déplace les démons dans le solutionnaire :
move(cos(moveAngle) * moveSpeed * sacredLightRatio * deltaTime, sin(moveAngle) *
moveSpeed * sacredLightRatio * deltaTime);
SacredLightRatio vaut 1 par défaut mais 0.5 si le démon est affecté par un sort de sacred light.  Ainsi la vitesse
est "sécurisée" et jamais directement modifiée par une source extérieure comme un sort.  Ceci empêche les bugs
indus.
*/

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

