/*
  ==============================================================================

    MotionBlockLibraryUI.cpp
    Created: 10 Apr 2018 10:58:08pm
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelLibraryUI.h"


MotionBlockModelLibraryUI::MotionBlockModelLibraryUI(const String &contentName, MotionBlockModelLibrary * library) :
	ShapeShifterContentComponent(contentName),
	rootItem(library),
    library(library)

{
	//iconSizeUI = library->iconSize->createSlider();
	//addAndMakeVisible(iconSizeUI);

	treeView.setDefaultOpenness(true);
	treeView.setRootItem(&rootItem);
	viewport.setViewedComponent(&treeView, false);
	viewport.setScrollBarsShown(true, false);
	viewport.setScrollOnDragEnabled(false);
	viewport.setScrollBarThickness(10);
	addAndMakeVisible(viewport);
}

MotionBlockModelLibraryUI::~MotionBlockModelLibraryUI()
{
}

void MotionBlockModelLibraryUI::paint(Graphics & g)
{
}

void MotionBlockModelLibraryUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	Rectangle<int> hr = r.removeFromTop(20);

//	iconSizeUI->setBounds(hr.removeFromRight(100).reduced(2));

	r.removeFromTop(2);
	int cy = r.getY();

	r.setY(0);
	r = r.withTrimmedRight(10);

	treeView.setSize(getWidth(), r.getBottom());
	viewport.setBounds(getLocalBounds().withTrimmedTop(cy));
}
