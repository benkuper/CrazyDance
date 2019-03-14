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


bool MotionBlockModelGroupTreeUI::keyPressed(const KeyPress & e)
{
	if (e.getKeyCode() == e.deleteKey || e.getKeyCode() == e.backspaceKey)
	{
		if (group != MotionBlockModelLibrary::getInstance() && inspectable->isSelected && inspectable->selectionManager == InspectableSelectionManager::activeSelectionManager)
		{
			if (group->askConfirmationBeforeRemove && GlobalSettings::getInstance()->askBeforeRemovingItems->boolValue())
			{
				int result = AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Delete " + group->niceName, "Are you sure you want to delete this ?", "Delete", "Cancel");
				if (result != 0) group->remove();
			}
			else
			{
				group->remove();
			}

			return true;
		}
	}

	return InspectableContentComponent::keyPressed(e);
}



/////////////// TREE ITEM


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
	case MotionBlockModelGroup::ModelGroupEvent::MODEL_ADDED:
	case MotionBlockModelGroup::ModelGroupEvent::MODEL_REMOVED:
	case MotionBlockModelGroup::ModelGroupEvent::GROUP_ADDED:
	case MotionBlockModelGroup::ModelGroupEvent::GROUP_REMOVED:
		rebuildTree();
		break;
	}
}
