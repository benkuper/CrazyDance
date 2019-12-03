/*
  ==============================================================================

	LaMoucheFolleIOInterface.cpp
	Created: 11 Mar 2019 6:45:57pm
	Author:  bkupe

  ==============================================================================
*/

#include "LaMoucheFolleIOInterface.h"
#include "Drone/Drone.h"
#include "Drone/DroneManager.h"

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
	String baseAddress = "/drones/" + d->droneID->stringValue();
	if (c == d->position) return baseAddress + "/flight/desiredPosition";
	else if (c == d->color) return baseAddress + "/lights/lightColor";
	else if (c == d->headlight) return baseAddress + "/lights/headlight";
	else return baseAddress + c->getControlAddress(d);
}

void LaMoucheFolleIOInterface::oscMessageReceived(const OSCMessage& m)
{
	OSCIOInterface::oscMessageReceived(m);

	StringArray tokens;
	tokens.addTokens(m.getAddressPattern().toString(), "/", "\"");

	if (tokens.size() < 3) return;

	if (tokens[1] == "drones")
	{
		int droneId = tokens[2].getIntValue();
		Drone* d = DroneManager::getInstance()->getDroneWithDroneId(droneId);

		if (d != nullptr)
		{
			if (m.size() >= 3)
			{
				if (tokens[tokens.size() - 1] == "realPosition") d->realPosition->setVector(m[0].getFloat32(), m[1].getFloat32(), m[2].getFloat32());
				if (tokens[tokens.size() - 1] == "desiredPosition") d->targetPosition->setVector(m[0].getFloat32(), m[1].getFloat32(), m[2].getFloat32());
			}
			
		}
	}

}
