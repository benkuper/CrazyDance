/*
  ==============================================================================

    MotionBlockModelGroup.h
    Created: 13 Apr 2018 2:03:02am
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class MotionBlockModelGroup :
	public ControllableContainer
{
public:
	MotionBlockModelGroup(const String &name) : ControllableContainer(name) {}
	~MotionBlockModelGroup() {}

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
};