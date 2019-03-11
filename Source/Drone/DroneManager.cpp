/*
  ==============================================================================

    DroneManager.cpp
    Created: 10 Mar 2019 8:49:55am
    Author:  bkupe

  ==============================================================================
*/

#include "DroneManager.h"

juce_ImplementSingleton(DroneManager)

DroneManager::DroneManager() :
	BaseManager("Drones")
{
}

DroneManager::~DroneManager()
{
}

void DroneManager::addItemInternal(Drone * d, var)
{
	d->addDroneListener(this);

	if (Engine::mainEngine->isLoadingFile) return;
	if (items.size() > 1) d->globalID->setValue(getFirstAvailableID());
}

void DroneManager::removeItemInternal(Drone * d)
{
	d->removeDroneListener(this);
}


Drone * DroneManager::getDroneWithId(int id, Drone * excludeDrone)
{
	for (auto & d : items)
	{
		if (d == excludeDrone) continue;
		if (d->globalID->intValue() == id) return d;
	}
	return nullptr;
}


int DroneManager::getFirstAvailableID()
{
	int numItems = items.size();
	for (int i = 0; i < numItems; i++)
	{
		if (getDroneWithId(i) == nullptr) return i;
	}
	return numItems;
}

void DroneManager::droneGlobalIDChanged(Drone * d, int previousID)
{
	Drone * otherDroneWithSameID = getDroneWithId(d->globalID->intValue(), d);
	if (otherDroneWithSameID != nullptr) otherDroneWithSameID->globalID->setValue(previousID);
}