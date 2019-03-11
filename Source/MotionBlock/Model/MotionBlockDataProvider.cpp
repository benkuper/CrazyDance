/*
  ==============================================================================

	MotionBlockDataProvider.cpp
	Created: 12 Apr 2018 2:10:56pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockDataProvider.h"
#include "Drone/DroneManager.h"

MotionBlockDataProvider::MotionBlockDataProvider(const String & name, bool canBeDisabled, bool canHaveScripts) :
	BaseItem(name, canBeDisabled, canHaveScripts)
{
	assignToAll = addTrigger("Assign To All", "Assign this Model or Preset to all props");
	viewUISize->isSavable = false;
}

MotionBlockDataProvider::~MotionBlockDataProvider() {
	MotionBlockDataProvider::masterReference.clear();
}


void MotionBlockDataProvider::onContainerTriggerTriggered(Trigger * t)
{
	BaseItem::onContainerTriggerTriggered(t);
	if (t == assignToAll)
	{
		for (auto & p : DroneManager::getInstance()->items)
		{
			p->activeProvider->setValueFromTarget(this);
		}
	}

}
