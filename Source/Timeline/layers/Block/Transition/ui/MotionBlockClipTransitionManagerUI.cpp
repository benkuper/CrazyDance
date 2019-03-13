/*
  ==============================================================================

    MotionBlockClipTransitionManagerUI.cpp
    Created: 13 Mar 2019 11:39:05am
    Author:  bkupe

  ==============================================================================
*/

#include "MotionBlockClipTransitionManagerUI.h"
#include "MotionBlockClipTransitionUI.h"
#include "../../ui/MotionBlockLayerTimeline.h"

MotionBlockClipTransitionManagerUI::MotionBlockClipTransitionManagerUI(MotionBlockLayerTimeline * timeline, MotionBlockClipTransitionManager * manager) :
	LayerBlockManagerUI(timeline, manager),
	manager(manager),
	timeline(timeline)
{
	autoFilterHitTestOnItems = true;
	validateHitTestOnNoItem = false;

	addExistingItems();
}

MotionBlockClipTransitionManagerUI::~MotionBlockClipTransitionManagerUI()
{
}

LayerBlockUI * MotionBlockClipTransitionManagerUI::createUIForItem(LayerBlock * item)
{
	return new MotionBlockClipTransitionUI((MotionBlockClipTransition *)item);
}
