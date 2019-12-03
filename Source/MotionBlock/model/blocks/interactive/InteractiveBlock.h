/*
  ==============================================================================

    InteractiveBlock.h
    Created: 28 Nov 2019 11:44:52am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../MotionBlockModel.h"

class InteractiveBlock :
	public MotionBlockModel
{
public:
	InteractiveBlock(var params = var());
	~InteractiveBlock();

	BoolParameter* useTargetPosition;

	void getMotionDataInternal(var result, Drone* d, double time, int id, var params, var * blockMemoryData) override;

	String getTypeString() const override { return "Interactive"; }
	static InteractiveBlock* create(var params) { return new InteractiveBlock(params); }
};