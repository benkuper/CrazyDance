/*
  ==============================================================================

    TimelineBlockUI.h
    Created: 23 Apr 2018 11:36:00am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../TimelineBlock.h"

class TimelineBlockUI :
	public MotionBlockModelUI
{
public:
	TimelineBlockUI(TimelineBlock * block);
	~TimelineBlockUI();

	TimelineBlock * tb;

	void editBlock() override;
};