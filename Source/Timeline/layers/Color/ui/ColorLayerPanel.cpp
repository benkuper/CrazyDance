/*
  ==============================================================================

    ColorLayerPanel.cpp
    Created: 10 Mar 2019 11:10:50am
    Author:  bkupe

  ==============================================================================
*/

#include "ColorLayerPanel.h"

ColorLayerPanel::ColorLayerPanel(ColorLayer * layer) :
	SequenceLayerPanel(layer),
	colorLayer(layer)
{
	colorUI = colorLayer->colorManager.currentColor->createColorParamUI();
	addAndMakeVisible(colorUI);
}

ColorLayerPanel::~ColorLayerPanel()
{
}

void ColorLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	colorUI->setBounds(r.removeFromTop(20));
}
