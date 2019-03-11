/*
  ==============================================================================

    DroneCluster.cpp
    Created: 5 Feb 2019 11:12:24am
    Author:  jonglissimo

  ==============================================================================
*/

#include "DroneCluster.h"
#include "../DroneManager.h"

DroneCluster::DroneCluster() :
	BaseItem("Cluster", false, false)
{
	userCanAddControllables = true;
	userAddControllablesFilters.add(IntParameter::getTypeStringStatic());
}

DroneCluster::~DroneCluster()
{
}

void DroneCluster::updateIDs()
{
	droneIDs.getLock().enter();
	droneIDs.clear();
	Array<WeakReference<Parameter>> params = getAllParameters();
	int index = 0;
	for (auto &p : params)
	{
		p->setNiceName("Drone ID " + String(index));
		if (p->type != Controllable::INT) continue;
		droneIDs.add(p->intValue());
		index++;
	}
	droneIDs.getLock().exit();
}

void DroneCluster::onContainerParameterChanged(Parameter *)
{
	updateIDs();
}

void DroneCluster::onControllableAdded(Controllable * c)
{
	c->isRemovableByUser = true;
	c->saveValueOnly = false;
	updateIDs();
}

void DroneCluster::onControllableRemoved(Controllable *)
{
	updateIDs();
}


Array<Drone*> DroneCluster::getDrones()
{
	Array<Drone*> result;

	droneIDs.getLock().enter();
	for (auto &p : DroneManager::getInstance()->items)
	{
		if (isDroneIsInCluster(p)) result.add(p);
	}
	droneIDs.getLock().exit();

	return result;
}

bool DroneCluster::isDroneIsInCluster(Drone * p) const
{
	return getLocalDroneID(p) >= 0;
}

int DroneCluster::getLocalDroneID(Drone * p) const
{
	droneIDs.getLock().enter();
	int result = droneIDs.indexOf(p->globalID->intValue());
	droneIDs.getLock().exit();
	return result;
}

void DroneCluster::loadJSONData(var data, bool createIfNotThere)
{
	ControllableContainer::loadJSONData(data, true); //force creating
}
