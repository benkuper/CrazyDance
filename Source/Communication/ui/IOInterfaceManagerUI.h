/*
  ==============================================================================

    IOInterfaceManagerUI.h
    Created: 11 Mar 2019 7:26:20pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../IOInterfaceManager.h"

class IOInterfaceManagerUI :
	public BaseManagerShapeShifterUI<IOInterfaceManager, IOInterface, BaseItemUI<IOInterface>>
{
public:
	IOInterfaceManagerUI(StringRef name);
	~IOInterfaceManagerUI();

	static IOInterfaceManagerUI * create(const String &name) { return new IOInterfaceManagerUI(name); }
};