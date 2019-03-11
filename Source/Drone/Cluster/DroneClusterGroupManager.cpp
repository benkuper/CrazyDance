/*
  ==============================================================================

    DroneClusterGroupManager.cpp
    Created: 5 Feb 2019 11:53:33am
    Author:  jonglissimo

  ==============================================================================
*/

#include "DroneClusterGroupManager.h"
#include "Drone/DroneManager.h"

juce_ImplementSingleton(DroneClusterGroupManager)

DroneClusterGroupManager::DroneClusterGroupManager() :
	BaseManager("Cluster Groups")
{
	DroneManager::getInstance()->addControllableContainerListener(this);
}

DroneClusterGroupManager::~DroneClusterGroupManager()
{
	if(DroneManager::getInstanceWithoutCreating() != nullptr) DroneManager::getInstance()->removeControllableContainerListener(this);
}