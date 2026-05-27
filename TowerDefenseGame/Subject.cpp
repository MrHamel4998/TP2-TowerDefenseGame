#include "Subject.h"
#include "IObserver.h"
#include <algorithm>

std::vector<IObserver*> Subject::observers;

void Subject::addObserver(IObserver* observer)
{
	if (observer == nullptr)
		return;

	if (std::find(observers.begin(), observers.end(), observer) == observers.end())
	{
		observers.push_back(observer);
	}
}

void Subject::removeAllObservers()
{
	observers.clear();
}

void Subject::removeObserver(IObserver* observer)
{
	auto iterator = std::find(observers.begin(), observers.end(), observer);
	if (iterator != observers.end())
	{
		observers.erase(iterator);
	}
}

void Subject::notifyAllObservers(EventType eventType)
{
	for (IObserver* observer : observers)
	{
		if (observer != nullptr)
		{
			observer->notify(this, eventType);
		}
	}
}
