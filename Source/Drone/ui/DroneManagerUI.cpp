/*
  ==============================================================================

    DroneManagerUI.cpp
    Created: 10 Mar 2019 8:50:06am
    Author:  bkupe

  ==============================================================================
*/

#include "DroneManagerUI.h"

DroneManagerUI::DroneManagerUI(StringRef name) :
	BaseManagerShapeShifterUI(name, DroneManager::getInstance())
{
	setDefaultLayout(HORIZONTAL);
	addExistingItems();
}

DroneManagerUI::~DroneManagerUI()
{
}
