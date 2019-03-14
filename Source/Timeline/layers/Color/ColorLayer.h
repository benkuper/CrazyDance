/*
  ==============================================================================

    ColorLayer.h
    Created: 10 Mar 2019 11:10:18am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "TimeColorManager.h"
#include "Drone/TargetFilter/DroneTargetFilterManager.h"

class Drone;

class ColorLayer :
	public SequenceLayer
{
public:
	ColorLayer(Sequence * s, var params);
	~ColorLayer();

	TimeColorManager colorManager;
	DroneTargetFilterManager filterManager;

	void sequenceCurrentTimeChanged(Sequence *, float, bool) override;
	void sequenceTotalTimeChanged(Sequence * s) override;

	Colour getColorAtTime(double time);

	SequenceLayerPanel * getPanel() override;
	SequenceLayerTimeline * getTimelineUI() override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static const String getTypeStringStatic() { return "Color"; }

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static ColorLayer * create(Sequence * s, var params) { return new ColorLayer(s, params); }
};