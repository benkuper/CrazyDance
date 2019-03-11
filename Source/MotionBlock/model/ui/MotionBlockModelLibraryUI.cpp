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
	library(library),
	genericGroupUI(&library->genericBlocks),
	scriptBlocksManagerUI("Scripts", &library->scriptBlocks),
	timelineBlocksManagerUI("Timelines",&library->timelineBlocks)
{
	iconSizeUI = library->iconSize->createSlider();
	addAndMakeVisible(iconSizeUI);

	viewport.setViewedComponent(&container, false);
	viewport.setScrollBarsShown(true, false);
	viewport.setScrollOnDragEnabled(false);
	viewport.setScrollBarThickness(10);
	addAndMakeVisible(viewport);

	genericGroupUI.setThumbSize(library->iconSize->intValue());
	scriptBlocksManagerUI.setThumbSize(library->iconSize->intValue());
	timelineBlocksManagerUI.setThumbSize(library->iconSize->intValue());


	container.addAndMakeVisible(&genericGroupUI);
	container.addAndMakeVisible(&scriptBlocksManagerUI);
	container.addAndMakeVisible(&timelineBlocksManagerUI);

	scriptBlocksManagerUI.addComponentListener(this);
	timelineBlocksManagerUI.addComponentListener(this);


	library->addAsyncCoalescedContainerListener(this);
}

MotionBlockModelLibraryUI::~MotionBlockModelLibraryUI()
{
	if (MotionBlockModelLibrary::getInstanceWithoutCreating() != nullptr) library->removeAsyncContainerListener(this);
}

void MotionBlockModelLibraryUI::paint(Graphics & g)
{
}

void MotionBlockModelLibraryUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	Rectangle<int> hr = r.removeFromTop(20);

	iconSizeUI->setBounds(hr.removeFromRight(100).reduced(2));

	r.removeFromTop(2);
	int cy = r.getY();

	r.setY(0);
	r = r.withTrimmedRight(10);

	if (genericGroupUI.getWidth() == 0) genericGroupUI.setBounds(r);
	genericGroupUI.setBounds(r.withHeight(genericGroupUI.getHeight()));
	r.translate(0, genericGroupUI.getHeight() + 10);

	scriptBlocksManagerUI.setBounds(r.withHeight(scriptBlocksManagerUI.getHeight()));
	r.translate(0, scriptBlocksManagerUI.getHeight() + 10);

	r.setHeight(timelineBlocksManagerUI.getHeight());
	timelineBlocksManagerUI.setBounds(r);

	container.setSize(getWidth(), r.getBottom());
	viewport.setBounds(getLocalBounds().withTrimmedTop(cy));
}

void MotionBlockModelLibraryUI::newMessage(const ContainerAsyncEvent & e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
	{
		if (e.targetControllable == library->iconSize)
		{
			genericGroupUI.setThumbSize(library->iconSize->intValue());
			scriptBlocksManagerUI.setThumbSize(library->iconSize->intValue());
			timelineBlocksManagerUI.setThumbSize(library->iconSize->intValue());

			resized();
		}
	}
	break;

	default:
		break;
	}
}

void MotionBlockModelLibraryUI::componentMovedOrResized(Component &, bool, bool)
{
	resized();
}
