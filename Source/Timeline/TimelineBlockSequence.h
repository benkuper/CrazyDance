/*
  ==============================================================================

    TimelineBlockSequence.h
    Created: 17 Apr 2018 8:30:13pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "layers/Block/MotionBlockLayer.h"

class MotionBlock;
class ColorLayer;
class Drone;

class TimelineBlockSequence :
	public Sequence,
	public SequenceLayerManager::Listener
{
public:
	TimelineBlockSequence();
	~TimelineBlockSequence();

	SequenceLayerFactory layerFactory;
	var getMotionData(Drone * p, double time, var params);

	BoolParameter * alwaysSendWhenPlaying;
	BoolParameter * sendTakeOffOnPlay;
	BoolParameter * sendLandOnStop;
	BoolParameter * sendLandOnPause;
	BoolParameter * sendLandOnFinish;

	Array<MotionBlockLayer *> getLayersForDrone(Drone * p, MotionBlockLayer::Mode mode, bool includeDisabled = false);
	ColorLayer * getColorLayerForDrone(Drone * p, bool includeDisabled = false);

	void itemAdded(SequenceLayer * s) override;

	void onContainerTriggerTriggered(Trigger  *t) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
};