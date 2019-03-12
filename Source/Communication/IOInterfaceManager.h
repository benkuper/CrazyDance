/*
  ==============================================================================

    IOInterfaceManager.h
    Created: 11 Mar 2019 6:45:15pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "IOInterface.h"

class IOInterfaceManager :
	public BaseManager<IOInterface>
{
public:
	juce_DeclareSingleton(IOInterfaceManager, true);

	IOInterfaceManager();
	~IOInterfaceManager();

	Factory<IOInterface> factory;

	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;
};