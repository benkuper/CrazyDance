/*
  ==============================================================================

    MotionBlockLibraryUI.h
    Created: 10 Apr 2018 10:58:08pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModelLibrary.h"
#include "MotionBlockModelManagerUI.h"
#include "MotionBlockModelGroupUI.h"

class MotionBlockModelLibraryUI :
	public ShapeShifterContentComponent,
	public ComponentListener,
	public ContainerAsyncListener
{
public:
	MotionBlockModelLibraryUI(const String &contentName, MotionBlockModelLibrary * library);
	~MotionBlockModelLibraryUI();

	Viewport viewport;
	Component container;

	ScopedPointer<IntSliderUI> iconSizeUI;

	MotionBlockModelLibrary * library;
	MotionBlockModelGroupUI genericGroupUI;

	MotionBlockModelManagerUI scriptBlocksManagerUI;
	MotionBlockModelManagerUI timelineBlocksManagerUI;


	void paint(Graphics &g) override;
	void resized() override;

	void newMessage(const ContainerAsyncEvent &e) override;

	void componentMovedOrResized(Component &, bool, bool) override;

	static MotionBlockModelLibraryUI * create(const String &contentName) { return new MotionBlockModelLibraryUI(contentName, MotionBlockModelLibrary::getInstance()); }

	
};