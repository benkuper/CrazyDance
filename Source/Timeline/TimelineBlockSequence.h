/*
  ==============================================================================

    TimelineBlockSequence.h
    Created: 17 Apr 2018 8:30:13pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class MotionBlock;
class MotionBlockLayer;
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

	MotionBlockLayer * getPrimaryLayerForDrone(Drone * p, bool includeDisabled = false);
	Array<MotionBlockLayer *> getSecondaryLayersForDrone(Drone * p, bool includeDisabled = false);

	void itemAdded(SequenceLayer * s) override;

	void onContainerTriggerTriggered(Trigger  *t) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
};