/*
  ==============================================================================

    IOInterface.cpp
    Created: 11 Mar 2019 6:45:20pm
    Author:  bkupe

  ==============================================================================
*/

#include "IOInterface.h"

IOInterface::IOInterface(StringRef name) :
	BaseItem(name)
{
	logIncoming = addBoolParameter("Log Incoming Data", "", false);
	logOutgoing = addBoolParameter("Log Outgoing Data", "", false);
}

IOInterface::~IOInterface()
{
}
