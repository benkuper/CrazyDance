/*
  ==============================================================================

    MotionBlockClipTransition.h
    Created: 13 Mar 2019 11:38:33am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MotionBlockClip.h"

class MotionBlockLayer;

class MotionBlockClipTransition :
	public LayerBlock
{
public:
	MotionBlockClipTransition(MotionBlockLayer * layer = nullptr, WeakReference<MotionBlockClip> fromClip = nullptr, WeakReference<MotionBlockClip> toClip = nullptr);
	~MotionBlockClipTransition();

	MotionBlockLayer * layer;

	WeakReference<MotionBlockClip> fromClip;
	WeakReference<MotionBlockClip> toClip;
	
	Automation curve;
	Automation fadeCurve;
	
	FloatParameter * fadeFrom;
	FloatParameter * fadeTo;

	FloatParameter * holdFrom;
	FloatParameter * holdTo;
	

	virtual void clearItem() override;

	void setFromClip(MotionBlockClip * clip);
	void setToClip(MotionBlockClip * clip);

	bool isInverted();

	void updateName();
	void resyncTimings();
	var getMotionData(Drone * p, double absoluteTime, var params);
	var getVarForPoint3D(Vector3D<float> val);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable *c) override;

};