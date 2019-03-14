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
	SequenceLayer(s, getTypeString()),
	colorManager(s->totalTime->floatValue())
{
	addChildControllableContainer(&colorManager);
	addChildControllableContainer(&filterManager);
}

ColorLayer::~ColorLayer()
{
}

void ColorLayer::sequenceCurrentTimeChanged(Sequence * s, float prevTime, bool skipData)
{
	SequenceLayer::sequenceCurrentTimeChanged(s, prevTime, skipData);
	colorManager.position->setValue(sequence->currentTime->floatValue());
}

void ColorLayer::sequenceTotalTimeChanged(Sequence * s)
{
	colorManager.setLength(sequence->totalTime->floatValue());
}

Colour ColorLayer::getColorAtTime(double time)
{
	return colorManager.getColorForPosition(time);
}

SequenceLayerPanel * ColorLayer::getPanel()
{
	return new ColorLayerPanel(this);
}

SequenceLayerTimeline * ColorLayer::getTimelineUI()
{
	return new ColorLayerTimeline(this);
}

var ColorLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();

	data.getDynamicObject()->setProperty("colorManager", colorManager.getJSONData());

	return data;
}

void ColorLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	colorManager.loadJSONData(data.getProperty("colorManager", var()));
}
