/*
  ==============================================================================

    MotionBlockLayerPanel.cpp
    Created: 17 Apr 2018 7:15:39pm
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockLayerPanel.h"

MotionBlockLayerPanel::MotionBlockLayerPanel(MotionBlockLayer * layer) :
	SequenceLayerPanel(layer),
	blockLayer(layer)
{
	modeUI = layer->mode->createUI();

	addAndMakeVisible(modeUI);
	minContentHeight = 80;
}

MotionBlockLayerPanel::~MotionBlockLayerPanel()
{
}

void MotionBlockLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	modeUI->setBounds(r.removeFromTop(16));
}
