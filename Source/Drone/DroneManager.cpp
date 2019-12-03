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
	lpsBox = addPoint3DParameter("LPS Box", "Bounds of the LPS");
	lpsBox->setBounds(0, 0, 0, 100, 100, 100);

	globalScale = addPoint3DParameter("Global Scale", "Basically multiplies the positions before setting the drone parameter.");
	globalScale->setVector(Vector3D<float>(1, 1, 1));

	globalOffset = addPoint3DParameter("Global Offset", "Basically offsets the positions before setting the drone parameter, but after the scale !.");
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

Drone* DroneManager::getDroneWithDroneId(int id, Drone* excludeDrone)
{
	for (auto& d : items)
	{
		if (d == excludeDrone) continue;
		if (d->droneID->intValue() == id) return d;
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