/*
  ==============================================================================

    MotionBlockClipManagerUI.h
    Created: 17 Apr 2018 7:20:49pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockClipManager.h"
#include "MotionBlockClipUI.h"

class MotionBlockLayerTimeline;

class MotionBlockClipManagerUI :
	public LayerBlockManagerUI
{
public:
	MotionBlockClipManagerUI(MotionBlockLayerTimeline * timeline, MotionBlockClipManager * manager);
	~MotionBlockClipManagerUI();

	MotionBlockClipManager * clipManager;
	MotionBlockLayerTimeline * timeline;

	int dropClipX;

	void paint(Graphics &g) override;

	LayerBlockUI * createUIForItem(LayerBlock * item) override;

	void itemDragEnter(const SourceDetails &source) override;
	void itemDragExit(const SourceDetails &source) override;
	void itemDragMove(const SourceDetails &source) override;
	void itemDropped(const SourceDetails &source) override;
};
