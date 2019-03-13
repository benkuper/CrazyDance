/*
  ==============================================================================

    MotionBlockClipManager.h
    Created: 17 Apr 2018 5:11:44pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlockClip.h"

class MotionBlockLayer;

class MotionBlockClipManager :
	public LayerBlockManager
{
public:
	MotionBlockClipManager(MotionBlockLayer * layer);
	~MotionBlockClipManager();

	MotionBlockLayer * layer;
	LayerBlock * createItem() override;

	void getSurroundingBlocks(MotionBlockClip * clip, MotionBlockClip *& prevClip, MotionBlockClip *& nextClip);

	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable *c) override;

	//void computeFadesForClip(MotionBlockClip * clip, bool propagate);
};