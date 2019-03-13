/*
  ==============================================================================

    MotionBlockLibraryUI.h
    Created: 10 Apr 2018 10:58:08pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModelLibrary.h"
#include "MotionBlockModelGroupTreeUI.h"

class MotionBlockModelLibraryUI :
	public ShapeShifterContentComponent
{
public:
	MotionBlockModelLibraryUI(const String &contentName, MotionBlockModelLibrary * library);
	~MotionBlockModelLibraryUI();

	Viewport viewport;
	TreeView treeView;
	GroupTreeViewItem rootItem;
	ScopedPointer<IntSliderUI> iconSizeUI;
	MotionBlockModelLibrary * library;

	void paint(Graphics &g) override;
	void resized() override;


	static MotionBlockModelLibraryUI * create(const String &contentName) { return new MotionBlockModelLibraryUI(contentName, MotionBlockModelLibrary::getInstance()); }
};