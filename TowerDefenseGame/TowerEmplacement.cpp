#include "TowerEmplacement.h"
#include "Subject.h"
#include "EventType.h"

TowerEmplacement::TowerEmplacement()
{
}

void TowerEmplacement::notify(Subject* subject, EventType eventType)
{
	if (eventType == EventType::TowerDeactivated)
	{
		// CH: La tour a été détruite, on peut mettre une autre tour à cet emplacement
		// Réinitialiser l'emplacement
	}
	else if (eventType == EventType::TowerActivated)
	{
		// CH: Une tour a été activée sur cet emplacement
		// Mettre l'emplacement comme occupé
	}
}