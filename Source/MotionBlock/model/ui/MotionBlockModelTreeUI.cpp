/*
  ==============================================================================

    MotionBlockModelTreeUI.cpp
    Created: 12 Mar 2019 6:20:35pm
    Author:  bkupe

  ==============================================================================
*/

#include "MotionBlockModelTreeUI.h"
#include "Drone/DroneManager.h"
#include "../blocks/timeline/TimelineBlock.h"

const Identifier MotionBlockModelTreeUI::dragAndDropID = "MotionBlockModel";

MotionBlockModelTreeUI::MotionBlockModelTreeUI(MotionBlockModel * model) :
	InspectableContentComponent(model),
	model(model)
{
	autoDrawContourWhenSelected = false;
}

MotionBlockModelTreeUI::~MotionBlockModelTreeUI()
{
}

void MotionBlockModelTreeUI::paint(Graphics & g)
{
	if (inspectable.wasObjectDeleted()) return;
	
	Colour textColor = BLUE_COLOR;
	if (model->isSelected)
	{
		g.setColour(BLUE_COLOR);
		g.fillRoundedRectangle(getLocalBounds().withWidth(g.getCurrentFont().getStringWidth(model->niceName)).expanded(10,0).toFloat(), 2);
		textColor = BG_COLOR;
	}

	g.setColour(textColor);
	g.drawFittedText(model->niceName, getLocalBounds().reduced(4), Justification::centredLeft, 1);
}


void MotionBlockModelTreeUI::mouseDown(const MouseEvent & e)
{
	InspectableContentComponent::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
		if (e.mods.isAltDown())
		{
			for (auto & d : DroneManager::getInstance()->items)
			{
				d->activeProvider->setValueFromTarget(model);
			}
		}
	}
	else if (e.mods.isRightButtonDown())
	{
		PopupMenu menu;
		PopupMenu assignMenu;

		int index = 1;
		for (auto & d : DroneManager::getInstance()->items)
		{
			assignMenu.addItem(index, d->niceName);
			index++;
		}

		menu.addItem(-1, "Assign to all");

		int result = menu.show();
		if (result == 0) return;
		if (result == -1)
		{
			for (auto & d : DroneManager::getInstance()->items)
			{
				d->activeProvider->setValueFromTarget(model);
			}
		}
	}
}

void MotionBlockModelTreeUI::mouseDoubleClick(const MouseEvent & e)
{
	InspectableContentComponent::mouseDoubleClick(e);
	//editBlock();
	
	if (model->getTypeString() == "Timeline")
	{
		ShapeShifterManager::getInstance()->showContent("Timeline Editor");
		TimeMachineView * view = (TimeMachineView *)ShapeShifterManager::getInstance()->getContentForName("Timeline Editor");
		if (view != nullptr)
		{
			TimelineBlock * tb = dynamic_cast<TimelineBlock *>(model);
			if(tb != nullptr) view->setSequence(&tb->sequence);
		}
	}
}

void MotionBlockModelTreeUI::mouseDrag(const MouseEvent & e)
{
	InspectableContentComponent::mouseDrag(e);

	if (isDragAndDropActive()) return;
	//Image dndImage = modelImage.rescaled(60, 60);
//	if (dndImage.getWidth() > 0 && dndImage.hasAlphaChannel()) dndImage.multiplyAllAlphas(.5f);

	if (e.getDistanceFromDragStart() > 40)
	{
		var desc = new DynamicObject();
		desc.getDynamicObject()->setProperty("type", dragAndDropID.toString());
		desc.getDynamicObject()->setProperty("model", model->getTypeString());
		startDragging(desc, this, Image(), true);
	}
}














ModelTreeViewItem::ModelTreeViewItem(MotionBlockModel * model) :
	InspectableContentComponent(model),
	model(model)
{
}

ModelTreeViewItem::~ModelTreeViewItem()
{
}

void ModelTreeViewItem::newMessage(const MotionBlockModel::ModelEvent & e)
{
}