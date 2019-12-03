/*
  ==============================================================================

    MotionBlockClipTransitionManager.cpp
    Created: 13 Mar 2019 11:38:24am
    Author:  bkupe

  ==============================================================================
*/

#include "MotionBlockClipTransitionManager.h"
#include "../MotionBlockLayer.h"

MotionBlockClipTransitionManager::MotionBlockClipTransitionManager(MotionBlockLayer * layer) :
	LayerBlockManager(layer, "Transitions"),
	layer(layer)
{
	userCanAddItemsManually = false;
}

MotionBlockClipTransitionManager::~MotionBlockClipTransitionManager()
{
}

void MotionBlockClipTransitionManager::getTransitionsForClip(MotionBlockClip * c, MotionBlockClipTransition *& beforeTransition, MotionBlockClipTransition *& afterTransition)
{
	jassert(c != nullptr);

	for (auto &i : items)
	{
		MotionBlockClipTransition * t = (MotionBlockClipTransition *)i;
		if (t->fromClip == c) afterTransition = t;
		if (t->toClip == c) beforeTransition = t;

		if (afterTransition != nullptr && beforeTransition != nullptr) break;
	}
}

MotionBlockClipTransition * MotionBlockClipTransitionManager::getBeforeTransition(MotionBlockClip * c)
{
	if (c == nullptr) return nullptr;
	for (auto &i : items)
	{
		MotionBlockClipTransition * t = (MotionBlockClipTransition *)i;
		if (t->toClip == c) return t;
	}

	return nullptr;
}


MotionBlockClipTransition * MotionBlockClipTransitionManager::getAfterTransition(MotionBlockClip * c)
{
	if (c == nullptr) return nullptr;
	for (auto &i : items)
	{
		MotionBlockClipTransition * t = (MotionBlockClipTransition *)i;
		if (t->fromClip == c) return t;
	}

	return nullptr;
}

LayerBlock * MotionBlockClipTransitionManager::createItem()
{
	return new MotionBlockClipTransition(layer);
}

MotionBlockClipTransition * MotionBlockClipTransitionManager::addTransition(MotionBlockClip * fromClip, MotionBlockClip * toClip)
{
	MotionBlockClipTransition * t = new MotionBlockClipTransition(layer, fromClip, toClip);
	addItem(t);
	return t;
}
