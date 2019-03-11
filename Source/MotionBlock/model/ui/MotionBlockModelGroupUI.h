/*
  ==============================================================================

    MotionBlockModelGroupUI.h
    Created: 10 Apr 2018 11:17:09pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModel.h"
#include "MotionBlockModelUI.h"

class MotionBlockModelGroupUI :
	public Component
{
public:
	MotionBlockModelGroupUI(ControllableContainer * group);
	~MotionBlockModelGroupUI();

	ControllableContainer * group;
	OwnedArray<MotionBlockModelUI> modelsUI;

	const int gap = 2;
	const int headerHeight = 16;
	const int headerGap = 4;
	int thumbSize = 48;

	void paint(Graphics &g) override;
	void resized() override;

	void setThumbSize(int value);
};