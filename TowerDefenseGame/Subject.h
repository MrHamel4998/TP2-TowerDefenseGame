#pragma once
#include <vector>
#include "Constants.h"

class IObserver;

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