/*
  ==============================================================================

    DroneClusterGroupManagerUI.h
    Created: 5 Feb 2019 4:37:13pm
    Author:  jonglissimo

  ==============================================================================
*/

#pragma once
#include "../DroneClusterGroupManager.h"

class DroneClusterGroupManagerUI :
	public BaseManagerShapeShifterUI<DroneClusterGroupManager, DroneClusterGroup, BaseItemUI<DroneClusterGroup>>
{
public:
	DroneClusterGroupManagerUI(const String &name, DroneClusterGroupManager * manager);
	~DroneClusterGroupManagerUI();

	static DroneClusterGroupManagerUI * create(const String &contentName) { return new DroneClusterGroupManagerUI(contentName, DroneClusterGroupManager::getInstance()); }
};