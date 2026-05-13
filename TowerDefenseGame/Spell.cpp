#include "Spell.h"

Spell::Spell()
{
}

void Spell::notifyCast()
{
	notifyAllObservers(EventType::SpellCast);
}