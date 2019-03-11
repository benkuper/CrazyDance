/*
  ==============================================================================

    ColorLayerPanel.h
    Created: 10 Mar 2019 11:10:50am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ColorLayer.h"

class ColorLayerPanel :
	public SequenceLayerPanel
{
public:
	ColorLayerPanel(ColorLayer * layer);
	~ColorLayerPanel();

	ColorLayer * colorLayer;
	ScopedPointer<ColorParameterUI> colorUI;

	void resizedInternalContent(Rectangle<int> &r) override;
};