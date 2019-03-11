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

class MotionBlockLayerTimeline :
	public SequenceLayerTimeline
{
public:
	MotionBlockLayerTimeline(MotionBlockLayer * layer);
	~MotionBlockLayerTimeline();

	MotionBlockLayer * blockLayer;
	MotionBlockClipManagerUI managerUI;

	void resized() override;
	void updateContent() override;

};