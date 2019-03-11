/*
  ==============================================================================

    MotionBlockModelManagerUI.h
    Created: 10 Apr 2018 7:56:53pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../UserMotionBlockModelManager.h"
#include "MotionBlockModelUI.h"

class MotionBlockModelManagerUI :
	public BaseManagerUI<UserMotionBlockModelManager, MotionBlockModel, MotionBlockModelUI>
{
public:
	MotionBlockModelManagerUI(const String &name, UserMotionBlockModelManager * manager);
	~MotionBlockModelManagerUI();

	int thumbSize = 48;
	const int gap = 2;
	const int headerHeight = 16;
	const int headerGap = 4;

	void paint(Graphics &g) override;
	void resized() override;

	void setThumbSize(int value);

	MotionBlockModelUI * createUIForItem(MotionBlockModel * i) override;

};