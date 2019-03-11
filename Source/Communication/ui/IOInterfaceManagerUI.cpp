/*
  ==============================================================================

    IOInterfaceManagerUI.cpp
    Created: 11 Mar 2019 7:26:20pm
    Author:  bkupe

  ==============================================================================
*/

#include "IOInterfaceManagerUI.h"

IOInterfaceManagerUI::IOInterfaceManagerUI(StringRef name) :
	BaseManagerShapeShifterUI(name, IOInterfaceManager::getInstance())
{
	addExistingItems();
}

IOInterfaceManagerUI::~IOInterfaceManagerUI()
{
}
