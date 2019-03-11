/*
  ==============================================================================

    IOInterface.h
    Created: 11 Mar 2019 6:45:20pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Drone;

class IOInterface :
	public BaseItem
{
public:
	IOInterface(StringRef name = "IO");
	virtual ~IOInterface();

	BoolParameter * logOutgoing;
	BoolParameter * logIncoming;
	
	virtual void sendDroneData(Drone * d, Controllable * c) {}

	virtual String getTypeString() const { jassertfalse;  return "IO"; }
};