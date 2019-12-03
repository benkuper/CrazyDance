/*
  ==============================================================================

    GenericActionTrigger.cpp
    Created: 7 Mar 2019 7:54:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericActionTrigger.h"

GenericActionTrigger::GenericActionTrigger() :
	Action("Generic")
{
	actionType = addEnumParameter("Type", "Type of action");
	actionType->addOption("Take off", TAKE_OFF)->addOption("Land", LAND);
}

GenericActionTrigger::~GenericActionTrigger()
{
}

void GenericActionTrigger::triggerInternal()
{
	Type t = actionType->getValueDataAsEnum<Type>();
	Array<Drone *> drones = cluster.getDrones();
	switch (t)
	{
	case TAKE_OFF:
		for (auto& d : drones) d->takeOff->trigger();
		break;

	case LAND:
		for (auto& d : drones) d->land->trigger();
		break;

	case HEADLIGHT_ON:
	case HEADLIGHT_OFF:
		for (auto& d : drones) d->headlight->setValue(t == HEADLIGHT_ON);
		break;
	}
}
