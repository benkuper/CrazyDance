/*
  ==============================================================================

    ColorLayer.cpp
    Created: 10 Mar 2019 11:10:18am
    Author:  bkupe

  ==============================================================================
*/

#include "ColorLayer.h"
#include "ui/ColorLayerPanel.h"
#include "ui/ColorLayerTimeline.h"

ColorLayer::ColorLayer(Sequence * s, var params) :
	SequenceLayer(s, params),
	colorManager(s->totalTime->floatValue())
{

}

ColorLayer::~ColorLayer()
{
}

void ColorLayer::sequenceCurrentTimeChanged(Sequence * s, float prevTime, bool skipData)
{
	SequenceLayer::sequenceCurrentTimeChanged(s, prevTime, skipData);
	colorManager.position->setValue(sequence->currentTime);
}

void ColorLayer::sequenceTotalTimeChanged(Sequence * s)
{
	colorManager.setLength(sequence->totalTime->floatValue());
}

SequenceLayerPanel * ColorLayer::getPanel()
{
	return new ColorLayerPanel(this);
}

SequenceLayerTimeline * ColorLayer::getTimelineUI()
{
	return new ColorLayerTimeline(this);
}
