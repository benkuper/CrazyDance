/*
  ==============================================================================

    DroneManagerUI.h
    Created: 10 Mar 2019 8:50:06am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../DroneManager.h"
#include "DroneUI.h"

class DroneManagerUI :
	public BaseManagerShapeShifterUI<DroneManager, Drone, DroneUI>
{
public:
	DroneManagerUI(StringRef name);
	~DroneManagerUI();

	static DroneManagerUI * create(const String &name) { return new DroneManagerUI(name); }
};