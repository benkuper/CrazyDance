/*
  ==============================================================================

    LaMoucheFolleIOInterface.h
    Created: 11 Mar 2019 6:45:57pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../OSCIOInterface.h"

class LaMoucheFolleIOInterface :
	public OSCIOInterface
{
public:
	LaMoucheFolleIOInterface(var params = var());
	~LaMoucheFolleIOInterface();


	String getAddressForDroneControllable(Drone * d, Controllable * c) const override;

	virtual void oscMessageReceived(const OSCMessage& m) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static const String getTypeStringStatic() { return "LaMoucheFolle";  }
	static IOInterface * create(var params) { return new LaMoucheFolleIOInterface(params); }
};