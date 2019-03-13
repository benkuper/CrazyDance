/*
  ==============================================================================

    MotionBlockModelGroupTreeUI.cpp
    Created: 12 Mar 2019 6:45:40pm
    Author:  bkupe

  ==============================================================================
*/

#include "MotionBlockModelGroupTreeUI.h"
#include "MotionBlockModelTreeUI.h"
#include "../MotionBlockModelLibrary.h"

MotionBlockModelGroupTreeUI::MotionBlockModelGroupTreeUI(MotionBlockModelGroup * group) :
	InspectableContentComponent(group),
	group(group)
{
	autoDrawContourWhenSelected = false;
}

MotionBlockModelGroupTreeUI::~MotionBlockModelGroupTreeUI()
{
}

void MotionBlockModelGroupTreeUI::paint(Graphics & g)
{
	if (inspectable.wasObjectDeleted()) return;
	
	Colour textColor = TEXT_COLOR;
	if (group->isSelected)
	{
		g.setColour(HIGHLIGHT_COLOR);
		g.fillRoundedRectangle(getLocalBounds().withWidth(g.getCurrentFont().getStringWidth(group->niceName)).expanded(10, 0).toFloat(), 2);
		textColor = BG_COLOR;
	}

	g.setColour(textColor);
	g.drawFittedText(group->niceName, getLocalBounds().reduced(4), Justification::centredLeft, 1);
}

void MotionBlockModelGroupTreeUI::mouseDown(const MouseEvent & e)
{
	InspectableContentComponent::mouseDown(e);

	if (e.mods.isRightButtonDown())
	{
		MotionBlockModelLibrary::getInstance()->showMenuAndCreateModelOrGroup(group);
	}
}

GroupTreeViewItem::GroupTreeViewItem(MotionBlockModelGroup * group) : 
	InspectableContentComponent(group),
	group(group)
{
	group->addAsyncCoalescedModelGroupListener(this);
	rebuildTree();
}

GroupTreeViewItem::~GroupTreeViewItem() 
{
	if (!inspectable.wasObjectDeleted()) group->removeAsyncModelGroupListener(this);
}

void GroupTreeViewItem::rebuildTree()
{
	clearSubItems();
	if (inspectable.wasObjectDeleted()) return;
	for (auto &i : group->modelManager.items)
	{
		addSubItem(new ModelTreeViewItem(i));
	}

	for (auto &i : group->groupManager.items)
	{
		addSubItem(new GroupTreeViewItem(i));
	}
}

void GroupTreeViewItem::newMessage(const MotionBlockModelGroup::ModelGroupEvent & e)
{
	switch (e.type)
	{
	case e.MODEL_ADDED:
	case e.MODEL_REMOVED:
	case e.GROUP_ADDED:
	case e.GROUP_REMOVED:
		rebuildTree();
		break;
	}
}
