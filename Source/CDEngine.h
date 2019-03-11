/*
  ==============================================================================

    CDEngine.h
    Created: 10 Mar 2019 9:05:52am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class CDEngine :
	public Engine,
	public OSCRemoteControl::RemoteControlListener
{
public:
	CDEngine();
	~CDEngine();

	ControllableContainer ioCC;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	OSCSender globalSender;

	void clearInternal() override;

	void processMessage(const OSCMessage &m) override;

	var getJSONData() override;
	void loadJSONDataInternalEngine(var data, ProgressTask * loadingTask) override;

};