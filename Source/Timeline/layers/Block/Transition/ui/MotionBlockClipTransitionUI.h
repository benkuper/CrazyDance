/*
  ==============================================================================

    MotionBlockClipTransitionUI.h
    Created: 13 Mar 2019 11:38:56am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MotionBlockClipTransition.h"

class MotionBlockClipTransitionUI :
	public LayerBlockUI
{
public:
	MotionBlockClipTransitionUI(MotionBlockClipTransition * transition);
	~MotionBlockClipTransitionUI();

	MotionBlockClipTransition * transition;

	void paint(Graphics &g) override;

	virtual bool hitTest(int x, int y) override;

};