/*
  ==============================================================================

	DroneTargetFilter.cpp
	Created: 5 Feb 2019 11:13:17am
	Author:  jonglissimo

  ==============================================================================
*/

#include "DroneTargetFilter.h"
#include "../DroneManager.h"
#include "Drone/Cluster/DroneClusterGroupManager.h"

DroneTargetFilter::DroneTargetFilter(StringRef name) :
	BaseItem(name)
{
	isSelectable = false;
}

DroneTargetFilter::~DroneTargetFilter()
{
}

bool DroneTargetFilter::isDroneValid(Drone * p)
{
	int droneCount = 0;
	return getTargetIDForDrone(p, droneCount) >= 0;
}

int DroneTargetFilter::getTargetIDForDrone(Drone * p, int &droneCount) 
{
	return -1;
}


DroneFilterGlobalID::DroneFilterGlobalID() :
	DroneTargetFilter(getTypeString())
{
	id = addIntParameter("ID", "Global ID to filter", 0);
}

DroneFilterGlobalID::~DroneFilterGlobalID()
{
}

int DroneFilterGlobalID::getTargetIDForDrone(Drone * p, int &droneCount) 
{
	return p->globalID->intValue() == id->intValue() ? id->intValue() : -1;
}


DroneFilterCluster::DroneFilterCluster() :
	DroneTargetFilter(getTypeString())
{
	specificClusterGroup = addBoolParameter("Specific Group", "Search in specific cluster Group", false);
	clusterGroup = addTargetParameter("Family", "Family to filter", DroneClusterGroupManager::getInstance());
	clusterGroup->targetType = TargetParameter::CONTAINER;
	clusterGroup->maxDefaultSearchLevel = 0;
	clusterGroup->showParentNameInEditor = false;
	clusterGroup->setEnabled(false);

	specificCluster = addBoolParameter("Specific Cluster", "Search in specific cluster", false);
	cluster = addTargetParameter("Cluster", "Cluster to filter", DroneClusterGroupManager::getInstance());
	cluster->targetType = TargetParameter::CONTAINER;
	cluster->maxDefaultSearchLevel = 2;
	cluster->defaultContainerTypeCheckFunc = &DroneFilterCluster::targetIsCluster;
	cluster->setEnabled(false);

	specificID = addBoolParameter("Specific Local ID", "Search for specific Local Cluster ID", false);
	id = addIntParameter("Local Drone ID", "The local cluster prop ID", 0);
	id->setEnabled(false);
}

DroneFilterCluster::~DroneFilterCluster()
{
}

int DroneFilterCluster::getTargetIDForDrone(Drone * p, int &droneCount) 
{
	int targetID = -1;

	if (specificClusterGroup->boolValue())
	{
		DroneClusterGroup * cg = dynamic_cast<DroneClusterGroup *>(clusterGroup->targetContainer.get());
		if (cg == nullptr) return -1;
		DroneCluster * c = cg->getClusterForDrone(p, targetID);
		if (c == nullptr) return -1;
		droneCount = c->droneIDs.size();

		//targetID = cg->getLocalDroneID(p);
	}
	else if (specificCluster->boolValue())
	{
		DroneCluster * c = dynamic_cast<DroneCluster *>(cluster->targetContainer.get());
		if (c == nullptr) return -1;
		targetID = c->getLocalDroneID(p);
		droneCount = c->droneIDs.size();
	}
	else
	{
		for (auto &cg : DroneClusterGroupManager::getInstance()->items)
		{
			int localID = -1;
			DroneCluster * c = cg->getClusterForDrone(p, localID);
			if (localID >= 0 && (!specificID->boolValue() || localID == id->intValue()))
			{
				targetID = localID;
				droneCount = c->droneIDs.size();
				break;
			}
		}
	}

	if (specificID->boolValue()) return targetID == id->intValue() ? targetID : -1;
	return targetID;
}

void DroneFilterCluster::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == specificClusterGroup)
	{
		clusterGroup->setEnabled(specificClusterGroup->boolValue());
		if (specificClusterGroup->boolValue()) specificCluster->setValue(false);
	}
	else if (p == specificCluster)
	{
		cluster->setEnabled(specificCluster->boolValue());
		if (specificCluster->boolValue()) specificClusterGroup->setValue(false);
	}
	else if (p == specificID)
	{
		id->setEnabled(specificID->boolValue());
	}
}

bool DroneFilterCluster::targetIsCluster(ControllableContainer * cc)
{
	return dynamic_cast<DroneCluster *>(cc) != nullptr;
}





DroneFilterScript::DroneFilterScript() :
	DroneTargetFilter("Script")
{
	addChildControllableContainer(&script);
}

DroneFilterScript::~DroneFilterScript()
{
}

int DroneFilterScript::getTargetIDForDrone(Drone * p, int &droneCount) 
{
	Array<var> args;
	args.add(p->getScriptObject());
	return script.callFunction("getID", args);
}
