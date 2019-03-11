/*
  ==============================================================================

    OSCIOInterface.h
    Created: 11 Mar 2019 6:45:42pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../IOInterface.h"

class OSCIOInterface :
	public IOInterface,
	public OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
public:
	OSCIOInterface(var params = var());
	virtual ~OSCIOInterface();

	IntParameter * localPort;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	OSCReceiver receiver;
	OSCSender sender;

	virtual void sendDroneData(Drone * d, Controllable *c) override;

	virtual String getAddressForDroneControllable(Drone * d, Controllable *c) const;

	virtual String getTypeString() const override { return getTypeStringStatic(); }
	static const String getTypeStringStatic() { return "OSC"; }
	static IOInterface * create(var params) { return new OSCIOInterface(params); }

	// Inherited via Listener
	virtual void oscMessageReceived(const OSCMessage & message) override;
};