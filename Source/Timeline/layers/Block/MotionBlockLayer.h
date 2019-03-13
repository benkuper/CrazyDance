/*
  ==============================================================================

    MotionBlockLayer.h
    Created: 17 Apr 2018 5:10:36pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlockClipManager.h"
#include "Drone/TargetFilter/DroneTargetFilterManager.h"
#include "Transition/MotionBlockClipTransitionManager.h"

class MotionBlockLayer :
	public SequenceLayer,
	public DroneTargetFilterManager::FilterManagerListener,
	public EngineListener,
	public MotionBlockClipManager::Listener
{
public:
	MotionBlockLayer(Sequence * s, var params = var());
	~MotionBlockLayer();

	enum Mode { MAIN, EFFECT};

	MotionBlockClipManager blockClipManager;
	MotionBlockClipTransitionManager transitionManager;

	//BoolParameter * defaultLayer;
	EnumParameter * mode;
	DroneTargetFilterManager filterManager;

	var getMotionData(Drone * d, double time, var params);

	void updateLinkedProps();

	void filtersChanged() override;
	void endLoadFile() override;

	void itemAdded(LayerBlock * clip) override;
	void itemRemoved(LayerBlock * clip) override;
	
	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	SequenceLayerPanel * getPanel() override;
	SequenceLayerTimeline * getTimelineUI() override;

	String getTypeString() const override { return "Motion"; }

	static MotionBlockLayer * create(Sequence * s, var params) { return new MotionBlockLayer(s, params);  }
};