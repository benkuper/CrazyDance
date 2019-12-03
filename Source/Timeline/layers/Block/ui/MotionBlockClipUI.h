/*
  ==============================================================================

    MotionBlockClipUI.h
    Created: 17 Apr 2018 7:20:43pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockClip.h"
#include "Drone/Drone.h"

class MotionBlockFadeHandle :
	public Component
{
public:
	MotionBlockFadeHandle(const Image &image);
	~MotionBlockFadeHandle() {}

	Image img;

	void paint(Graphics &g) override;
};

class MotionBlockClipUI :
	public LayerBlockUI
	//public Thread //Threaded preview generation
{
public:
	MotionBlockClipUI(MotionBlockClip * clip);
	~MotionBlockClipUI();

	MotionBlockClip * clip;

	ScopedPointer<AutomationUI> automationUI;

	MotionBlockFadeHandle fadeInHandle;
	MotionBlockFadeHandle fadeOutHandle;

	//
	float fadeValueAtMouseDown;

	//Prop previewProp;
	bool shouldUpdateImage;
	bool imageIsReady;

	SpinLock imgLock;
	Image previewImage;

	bool isDraggingModel;

	void paint(Graphics &g) override;
	void paintOverChildren(Graphics &g) override;

	void resizedBlockInternal() override;

	void generatePreview();

	void setTargetAutomation(ParameterAutomation * a);

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;

	Rectangle<int> getDragBounds() override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	void itemDragEnter(const SourceDetails &source) override;
	void itemDragExit(const SourceDetails &source) override;
	void itemDragMove(const SourceDetails &source) override;
	void itemDropped(const SourceDetails &source) override;


	//void run() override;

};
