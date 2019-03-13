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
	managerUI(this, &layer->blockClipManager),
	transitionManagerUI(this, &layer->transitionManager)
{
	addAndMakeVisible(managerUI);
	managerUI.bringToFrontOnSelect = false;
	addAndMakeVisible(transitionManagerUI);
	transitionManagerUI.bringToFrontOnSelect = false;
}

MotionBlockLayerTimeline::~MotionBlockLayerTimeline()
{
}

void MotionBlockLayerTimeline::resized()
{
	managerUI.setBounds(getLocalBounds());
	transitionManagerUI.setBounds(getLocalBounds());
}

void MotionBlockLayerTimeline::updateContent()
{
	managerUI.updateContent();
	transitionManagerUI.updateContent();
}
