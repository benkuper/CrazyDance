/*
  ==============================================================================

    MotionBlockModelPreset.h
    Created: 12 Apr 2018 1:10:55pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../MotionBlockDataProvider.h"

class MotionBlockModel;

class MotionBlockModelPreset :
	public MotionBlockDataProvider
{
public:
	MotionBlockModelPreset(MotionBlockModel * model = nullptr, var params = var());
	virtual ~MotionBlockModelPreset();

	MotionBlockModel * model;
	WeakReference<Inspectable> modelRef;
	ControllableContainer paramsContainer;

	var previousData; //for keeping overriden value and saved dynamic values
	var getMotionData(Drone * p, double time, var params) override;

	void rebuildArgsFromModel(bool syncValues = true);
	void childStructureChanged(ControllableContainer * cc) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable *) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	Array<WeakReference<Controllable>> getModelParameters() override;
		
};