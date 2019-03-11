/*
  ==============================================================================

    ColorLayerTimeline.h
    Created: 10 Mar 2019 11:10:44am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ColorLayer.h"
#include "TimeColorManagerUI.h"

class ColorLayerTimeline :
	public SequenceLayerTimeline
{
public:
	ColorLayerTimeline(ColorLayer * layer);
	~ColorLayerTimeline();

	ColorLayer * colorLayer;
	TimeColorManagerUI colorManagerUI;

	void resized() override;
	void updateContent() override;
};