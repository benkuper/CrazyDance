/*
  ==============================================================================

    MotionBlockClipTransitionManager.h
    Created: 13 Mar 2019 11:38:24am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "MotionBlockClipTransition.h"
#include "../MotionBlockClipManager.h"

class MotionBlockLayer;

class MotionBlockClipTransitionManager :
	public LayerBlockManager
{
public:
	MotionBlockClipTransitionManager(MotionBlockLayer * layer);
	~MotionBlockClipTransitionManager();

	MotionBlockLayer * layer;

	void getTransitionsForClip(MotionBlockClip * c, MotionBlockClipTransition *&beforeTransition, MotionBlockClipTransition *&afterTransition);
	MotionBlockClipTransition * getBeforeTransition(MotionBlockClip * c);
	MotionBlockClipTransition * getAfterTransition(MotionBlockClip * c);
	
	LayerBlock * createItem() override;
	MotionBlockClipTransition * addTransition(MotionBlockClip * fromClip, MotionBlockClip * toClip);
};