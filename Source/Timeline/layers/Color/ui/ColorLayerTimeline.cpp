/*
  ==============================================================================

    ColorLayerTimeline.cpp
    Created: 10 Mar 2019 11:10:44am
    Author:  bkupe

  ==============================================================================
*/

#include "ColorLayerTimeline.h"

ColorLayerTimeline::ColorLayerTimeline(ColorLayer * layer) :
	SequenceLayerTimeline(layer),
	colorLayer(layer),
	colorManagerUI(&layer->colorManager)
{
	addAndMakeVisible(&colorManagerUI);
}

ColorLayerTimeline::~ColorLayerTimeline()
{
}

void ColorLayerTimeline::resized()
{
	colorManagerUI.setBounds(getLocalBounds());
}

void ColorLayerTimeline::updateContent()
{
	colorManagerUI.setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
}
