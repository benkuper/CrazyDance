/*
  ==============================================================================

	LaMoucheFolleIOInterface.cpp
	Created: 11 Mar 2019 6:45:57pm
	Author:  bkupe

  ==============================================================================
*/

#include "LaMoucheFolleIOInterface.h"
#include "Drone/Drone.h"

LaMoucheFolleIOInterface::LaMoucheFolleIOInterface(var params) :
	OSCIOInterface(getTypeString(), params)
{
}

LaMoucheFolleIOInterface::~LaMoucheFolleIOInterface()
{
}

String LaMoucheFolleIOInterface::getAddressForDroneControllable(Drone * d, Controllable * c) const
{
	String baseAddress = "/drone-" + d->droneID->stringValue() + "/";
	if (c == d->position) return baseAddress + "desiredPosition";
	else if (c == d->color) return baseAddress + "lightColor";
	else if (c == d->headlight) return baseAddress + "headlight";
	else return baseAddress + c->shortName;
}
