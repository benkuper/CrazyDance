/*
  ==============================================================================

    MotionBlockModelUI.cpp
    Created: 10 Apr 2018 8:10:04pm
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelUI.h"
#include "Drone/DroneManager.h"

const Identifier MotionBlockModelUI::dragAndDropID = "MotionBlockModel";

MotionBlockModelUI::MotionBlockModelUI(MotionBlockModel * model) :
	 BaseItemMinimalUI(model)
{
	bgColor = item->isBeingEdited ? BLUE_COLOR.darker().withSaturation(.3f) : bgColor = BG_COLOR.brighter(.1f);
	
	updateThumbnail();
	
	setSize(64, 64);

	setRepaintsOnMouseActivity(true);
	removeMouseListener(this);

	model->addAsyncModelListener(this);
} 

MotionBlockModelUI::~MotionBlockModelUI()
{
	if (item != nullptr) item->removeAsyncModelListener(this);
}

void MotionBlockModelUI::paint(Graphics & g)
{
	g.setColour(bgColor);
	g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	g.setColour(Colours::white.withAlpha(isMouseOver() ? .2f : 1.f));
	if (modelImage.getWidth() > 0) g.drawImage(modelImage, getLocalBounds().reduced(6).toFloat());

	if (modelImage.getWidth() == 0 || isMouseOver())
	{
		g.setColour(Colours::white);
		g.drawFittedText(item->niceName, getLocalBounds().reduced(4), Justification::centred, 3);
	}
}

void MotionBlockModelUI::resized()
{
}

void MotionBlockModelUI::updateThumbnail()
{
	if (item->customThumbnailPath.isNotEmpty())
	{
		modelImage = ImageCache::getFromFile(item->customThumbnailPath);
	}

	if(modelImage.getWidth() == 0)
	{
		int numBytes;
		const char * imgData = BinaryData::getNamedResource((StringUtil::toShortName(item->getTypeString()) + "_png").getCharPointer(), numBytes);
		modelImage = ImageCache::getFromMemory(imgData, numBytes);
	}


	repaint();
}


void MotionBlockModelUI::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);


	if (e.mods.isLeftButtonDown())
	{
		if (e.mods.isAltDown())
		{
			for (auto & d : DroneManager::getInstance()->items)
			{
				d->activeProvider->setValueFromTarget(item);
			}
		}
	}else if (e.mods.isRightButtonDown())
	{
		PopupMenu menu;
		PopupMenu assignMenu;

		int index = 1;
		for (auto & d: DroneManager::getInstance()->items)
		{
			assignMenu.addItem(index, d->niceName);
			index++;
		}
		
		menu.addItem(-1, "Assign to all");
		menu.addSubMenu("Assign to...", assignMenu);
		
		int result = menu.show();
		if (result == 0) return;
		if (result == -1)
		{
			for (auto & d : DroneManager::getInstance()->items)
			{
				d->activeProvider->setValueFromTarget(item);
			}
		}if (result > 0 && result <= DroneManager::getInstance()->items.size())
		{
			Drone * d = DroneManager::getInstance()->items[result - 1];
			d->activeProvider->setValueFromTarget(item);
		}
	}
}

void MotionBlockModelUI::mouseDoubleClick(const MouseEvent & e)
{
	editBlock();
}

void MotionBlockModelUI::mouseDrag(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDrag(e);

	if (isDragAndDropActive()) return;
	Image dndImage = modelImage.rescaled(60, 60);
	if(dndImage.getWidth() > 0 && dndImage.hasAlphaChannel()) dndImage.multiplyAllAlphas(.5f);

	if (e.getDistanceFromDragStart() > 40)
	{
		var desc = new DynamicObject();
		desc.getDynamicObject()->setProperty("type", dragAndDropID.toString());
		desc.getDynamicObject()->setProperty("model", item->getTypeString());
		startDragging(desc, this, dndImage, true);
	}
}


void MotionBlockModelUI::newMessage(const MotionBlockModel::ModelEvent & e)
{
	switch (e.type)
	{
	case MotionBlockModel::ModelEvent::CUSTOM_THUMBNAIL_CHANGED:
		updateThumbnail();
		break;

	case MotionBlockModel::ModelEvent::EDITING_STATE_CHANGED:
		bgColor = item->isBeingEdited ? BLUE_COLOR.darker().withSaturation(.3f) : bgColor = BG_COLOR.brighter(.1f);
		repaint();
		break;
	}
}
