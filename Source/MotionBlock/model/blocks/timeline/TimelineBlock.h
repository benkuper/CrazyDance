/*
  ==============================================================================

    TimelineBlock.h
    Created: 10 Apr 2018 6:59:02pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MotionBlockModel.h"
#include "Timeline/TimelineBlockSequence.h"

class LightBlock;

class TimelineBlock :
	public MotionBlockModel,
	public Sequence::SequenceListener
{
public:
	TimelineBlock(var params = var());
	~TimelineBlock();

	TimelineBlockSequence sequence;

	var getMotionData(Drone * d, double time, var params, var * blockMemoryData) override;

	void sequenceCurrentTimeChanged(Sequence *, float /*prevTime*/, bool /*evaluateSkippedData*/) override;
	void sequencePlayStateChanged(Sequence * s) override;
	void sequenceEditingStateChanged(Sequence *s) override;

	void sequenceLooped(Sequence * s) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	String getTypeString() const override { return "Timeline"; }
	static TimelineBlock * create(var params) { return new TimelineBlock(params); }
};