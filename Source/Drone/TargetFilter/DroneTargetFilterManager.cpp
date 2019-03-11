/*
  ==============================================================================

    DroneTargetFilterManager.cpp
    Created: 5 Feb 2019 12:43:58pm
    Author:  jonglissimo

  ==============================================================================
*/

#include "DroneTargetFilterManager.h"
#include "DroneTargetFilter.h"

DroneTargetFilterManager::DroneTargetFilterManager() :
	BaseManager<DroneTargetFilter>("Filters")
{
	selectItemWhenCreated = false;
	managerFactory = &factory;
	factory.defs.add(Factory<DroneTargetFilter>::Definition::createDef("", "Global ID", DroneFilterGlobalID::create));
	factory.defs.add(Factory<DroneTargetFilter>::Definition::createDef("", "Cluster", DroneFilterCluster::create));
	factory.defs.add(Factory<DroneTargetFilter>::Definition::createDef("", "Script", DroneFilterScript::create));
}

DroneTargetFilterManager::~DroneTargetFilterManager()
{

}

int DroneTargetFilterManager::getTargetIDForDrone(Drone * p, int &droneCount)
{
	int numProcessed = 0;
	for (auto &f : items)
	{
		if (!f->enabled->boolValue()) continue;
		int result = f->getTargetIDForDrone(p, droneCount);
		if (result >= 0) return result;
		numProcessed++;
	}

	if (numProcessed == 0) return p->globalID->intValue();
	return -1;
}

void DroneTargetFilterManager::addItemInternal(DroneTargetFilter *, var)
{
	filerManagerListeners.call(&FilterManagerListener::filtersChanged);
}

void DroneTargetFilterManager::removeItemInternal(DroneTargetFilter *)
{
	filerManagerListeners.call(&FilterManagerListener::filtersChanged);
}

void DroneTargetFilterManager::onControllableFeedbackUpdate(ControllableContainer *, Controllable * c)
{
	filerManagerListeners.call(&FilterManagerListener::filtersChanged);
}
