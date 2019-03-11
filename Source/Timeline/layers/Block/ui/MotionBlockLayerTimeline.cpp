/*
  ==============================================================================

	MotionBlockLayerTimeline.cpp
	Created: 17 Apr 2018 7:15:44pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockLayerTimeline.h"

MotionBlockLayerTimeline::MotionBlockLayerTimeline(MotionBlockLayer * layer) :
	SequenceLayerTimeline(layer),
	managerUI(this, &layer->blockClipManager)
{
	addAndMakeVisible(managerUI);
}

MotionBlockLayerTimeline::~MotionBlockLayerTimeline()
{
}

void MotionBlockLayerTimeline::resized()
{
	managerUI.setBounds(getLocalBounds());
}

void MotionBlockLayerTimeline::updateContent()
{
	managerUI.updateContent();
}
