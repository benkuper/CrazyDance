/*
  ==============================================================================

    DroneClusterGroupManager.h
    Created: 5 Feb 2019 11:53:33am
    Author:  jonglissimo

  ==============================================================================
*/

#pragma once

#include "DroneClusterGroup.h"

class DroneClusterGroupManager :
	public BaseManager<DroneClusterGroup>
{
public:
	juce_DeclareSingleton(DroneClusterGroupManager, true);

	DroneClusterGroupManager();
	~DroneClusterGroupManager();
};