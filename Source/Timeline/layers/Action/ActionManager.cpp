/*
  ==============================================================================

    ActionManager.cpp
    Created: 7 Mar 2019 7:55:04pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionManager.h"
#include "ActionLayer.h"
#include "GenericActionTrigger.h"

ActionManager::ActionManager(ActionLayer * layer, Sequence * sequence) :
	TimeTriggerManager(layer, sequence)
{
	factory.defs.add(Factory<TimeTrigger>::Definition::createDef("", "Generic", &GenericActionTrigger::create));
	managerFactory = &factory;
}

ActionManager::~ActionManager()
{
}
