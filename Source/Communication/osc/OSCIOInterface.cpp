/*
  ==============================================================================

    OSCIOInterface.cpp
    Created: 11 Mar 2019 6:45:42pm
    Author:  bkupe

  ==============================================================================
*/

#include "OSCIOInterface.h"
#include "Drone/Drone.h"

OSCIOInterface::OSCIOInterface(StringRef name, var params) :
	IOInterface(name)
{
	localPort = addIntParameter("Local Port", "", 16001, 1024, 65535);
	remoteHost = addStringParameter("Remote Host", "", "127.0.0.1");
	remotePort = addIntParameter("Remote Port", "", 16000, 1024, 65535);

	sender.connect("0.0.0.0", 1024);
}

OSCIOInterface::~OSCIOInterface()
{
}

void OSCIOInterface::sendDroneData(Drone * d, Controllable * c)
{
	String address = getAddressForDroneControllable(d, c);
	OSCMessage m(address);
	if (c->type != Controllable::TRIGGER)
	{
		Parameter * p = (Parameter *)c;
		if (p->type == Controllable::COLOR) m.addArgument(OSCHelpers::getOSCColour(((ColorParameter *)c)->getColor()));
		else if (p->value.isArray())
		{
			for (int i = 0; i < p->value.size(); i++) m.addArgument(OSCHelpers::varToArgument(p->value[i]));
		}
		else
		{
			m.addArgument(OSCHelpers::varToArgument(p->value));
		}
	}

	if (logOutgoing->boolValue())
	{
		NLOG(niceName, "Send " + address + " " + (c->type == Controllable::TRIGGER?"":((Parameter *)c)->stringValue()));
	}

	sender.sendToIPAddress(remoteHost->stringValue(), remotePort->intValue(), m);
}

String OSCIOInterface::getAddressForDroneControllable(Drone * d, Controllable * c) const
{
	return "/drone/" + d->droneID->stringValue() + "/" + c->getControlAddress(d);
}

void OSCIOInterface::oscMessageReceived(const OSCMessage & message)
{
	//
}
