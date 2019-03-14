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
	localPort->setValue(13001);
	remotePort->setValue(13000);
}

LaMoucheFolleIOInterface::~LaMoucheFolleIOInterface()
{
}

String LaMoucheFolleIOInterface::getAddressForDroneControllable(Drone * d, Controllable * c) const
{
	String baseAddress = "/drone-" + d->droneID->stringValue();
	if (c == d->position) return baseAddress + "/flight/desiredPosition";
	else if (c == d->color) return baseAddress + "/lights/lightColor";
	else if (c == d->headlight) return baseAddress + "/lights/headlight";
	else return baseAddress + c->getControlAddress(d);
}
