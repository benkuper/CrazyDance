/*
  ==============================================================================

    IOInterfaceManager.cpp
    Created: 11 Mar 2019 6:45:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "IOInterfaceManager.h"
#include "Drone/DroneManager.h"

#include "osc/OSCIOInterface.h"
#include "osc/lamouchefolle/LaMoucheFolleIOInterface.h"
#include "ui\IOInterfaceManagerUI.h"

juce_ImplementSingleton(IOInterfaceManager)

IOInterfaceManager::IOInterfaceManager() :
	BaseManager("Interfaces")
{
	factory.defs.add(Factory<IOInterface>::Definition::createDef("", OSCIOInterface::getTypeStringStatic(), &OSCIOInterface::create));
	managerFactory = &factory;
	DroneManager::getInstance()->addControllableContainerListener(this);
}

IOInterfaceManager::~IOInterfaceManager()
{
	if (DroneManager::getInstanceWithoutCreating() != nullptr) DroneManager::getInstance()->removeControllableContainerListener(this);
}

void IOInterfaceManager::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	if (cc == DroneManager::getInstance())
	{
		Drone * d = ControllableUtil::findParentAs<Drone>(c, 3);
		if (d != nullptr)
		{

			for (auto &i : items) i->sendDroneData(d, c);
		}
		return;
	}

	BaseManager::onControllableFeedbackUpdate(cc, c);
}
