/*
  ==============================================================================

    MotionBlockLayerPanel.h
    Created: 17 Apr 2018 7:15:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockLayer.h"

class MotionBlockLayerPanel :
	public SequenceLayerPanel
{
public:
	MotionBlockLayerPanel(MotionBlockLayer * layer);
	~MotionBlockLayerPanel();

	MotionBlockLayer * blockLayer;

	ScopedPointer<EnumParameterUI> modeUI;

	void resizedInternalContent(Rectangle<int> &r) override;
};
