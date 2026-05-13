#pragma once
#include <vector>
#include "EventType.h"

class IObserver;

// Universel; à récupérer tel quel.
class Subject
{
public:
	static void addObserver(IObserver* observer);
	static void removeObserver(IObserver* observer);
	static void removeAllObservers();

	virtual void notifyAllObservers(EventType eventType);

private:
	static std::vector<IObserver*> observers;
};
