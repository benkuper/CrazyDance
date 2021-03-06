/*
  ==============================================================================

    MotionBlockClip.h
    Created: 17 Apr 2018 5:10:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlock/MotionBlock.h"

class Drone;
class MotionBlockLayer;

class MotionBlockClip :
	public LayerBlock,
	public MotionBlock::MotionBlockListener
{
public:
	MotionBlockClip(MotionBlockLayer * layer = nullptr, float time = 0);
	~MotionBlockClip();

	MotionBlockLayer * layer;
	ScopedPointer<MotionBlock> currentBlock;
	OwnedArray<MotionBlock> secondaryBlocks;

	TargetParameter * activeProvider;


	FloatParameter* clipStartOffset;
	BoolParameter * autoFade;
	FloatParameter * fadeIn;
	FloatParameter * fadeOut;
	Automation fadeCurve;

	var blockData; //for ghosting

	void setBlockFromProvider(MotionBlockDataProvider* provider);
	var getMotionData(Drone * p, double absoluteTime, var params);

	virtual void blockParamControlModeChanged(Parameter *p) override;

	//void itemAdded(ParameterAutomation * p) override;

	void setCoreLength(float value, bool stretch = false, bool stickToCoreEnd = false) override;

	void onContainerParameterChangedInternal(Parameter * p) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	String getTypeString() const override { return "MotionBlockClip"; }

	WeakReference<MotionBlockClip>::Master masterReference;
};