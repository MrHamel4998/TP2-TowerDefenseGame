#pragma once
#include "EventType.h"

class Subject;

class IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void notify(Subject* subject, EventType eventType) = 0;
};
