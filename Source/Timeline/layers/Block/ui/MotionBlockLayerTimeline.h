/*
  ==============================================================================

    MotionBlockLayerTimeline.h
    Created: 17 Apr 2018 7:15:44pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockLayer.h"
#include "MotionBlockClipManagerUI.h"
#include "../Transition/ui/MotionBlockClipTransitionManagerUI.h"

class MotionBlockLayerTimeline :
	public SequenceLayerTimeline
{
public:
	MotionBlockLayerTimeline(MotionBlockLayer * layer);
	~MotionBlockLayerTimeline();

	MotionBlockLayer * blockLayer;
	MotionBlockClipManagerUI managerUI;
	MotionBlockClipTransitionManagerUI transitionManagerUI;

	void resized() override;
	void updateContent() override;

};