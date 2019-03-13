/*
  ==============================================================================

    MotionBlockClipTransitionManagerUI.h
    Created: 13 Mar 2019 11:39:05am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MotionBlockClipTransitionManager.h"

class MotionBlockLayerTimeline;

class MotionBlockClipTransitionManagerUI :
	public LayerBlockManagerUI
{
public:
	MotionBlockClipTransitionManagerUI(MotionBlockLayerTimeline * timeline, MotionBlockClipTransitionManager * manager);
	~MotionBlockClipTransitionManagerUI();

	MotionBlockClipTransitionManager * manager;
	MotionBlockLayerTimeline * timeline;
	

	LayerBlockUI * createUIForItem(LayerBlock * item) override;
};