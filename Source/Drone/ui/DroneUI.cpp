/*
  ==============================================================================

    DroneUI.cpp
    Created: 10 Mar 2019 8:50:00am
    Author:  bkupe

  ==============================================================================
*/

#include "DroneUI.h"
#include "MotionBlock/model/library/ui/MotionBlockModelTreeUI.h"

DroneUI::DroneUI(Drone * d) :
	BaseItemUI(d),
	isDraggingItemOver(false)
{
	item->addAsyncCoalescedDroneListener(this);
	updateDroneImage();
	updateBlockImage();
	setSize(100, 100);
}

DroneUI::~DroneUI()
{
	if(!inspectable.wasObjectDeleted()) item->removeAsyncDroneListener(this);
}

void DroneUI::paint(Graphics & g)
{
	BaseItemUI::paint(g);
	Rectangle<int> r = getMainBounds().withTop(headerHeight + headerGap);

	g.setColour(item->color->getColor());
	g.fillEllipse(getLocalBounds().withSizeKeepingCentre(30,30).toFloat());

	//g.setColour(item->uiColor->getColor());
	if (blockImage.getWidth() > 0) g.drawImage(blockImage, r.removeFromBottom(r.getHeight() / 3).toFloat(), RectanglePlacement::centred);
	if (droneImage.getWidth() > 0) g.drawImage(droneImage, r.toFloat().reduced(8), RectanglePlacement::centred);

	g.setColour(TEXT_COLOR);
	g.drawText(String(item->globalID->intValue()), getMainBounds().removeFromRight(20).removeFromBottom(20).toFloat(), Justification::centred);
}

void DroneUI::updateDroneImage()
{
	Drone::State s = item->state->getValueDataAsEnum<Drone::State>();
	String stateString = item->stateStrings[(int)s];

	int numBytes;
	const char * imgData = BinaryData::getNamedResource(("drone_" + stateString.toLowerCase() + "_png").getCharPointer(), numBytes);
	droneImage = ImageCache::getFromMemory(imgData, numBytes);
	repaint();
}

void DroneUI::updateBlockImage()
{
	if (item->currentBlock == nullptr)
	{
		blockImage = Image();
		return;
	}
	int numBytes;
	const char * imgData = BinaryData::getNamedResource( (StringUtil::toShortName(item->currentBlock->provider->getTypeString()) + "_png").getCharPointer(), numBytes);
	blockImage = ImageCache::getFromMemory(imgData, numBytes);
	repaint();
}

void DroneUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->state) updateDroneImage();
	else if (c == item->globalID) repaint();
}

void DroneUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI::mouseDown(e);
	if (e.mods.isShiftDown()) item->takeOff->trigger();
	else if (e.mods.isCtrlDown()) item->land->trigger();
}

void DroneUI::newMessage(const Drone::DroneEvent & e)
{
	switch (e.type)
	{
	case Drone::DroneEvent::BLOCK_CHANGED:
		updateBlockImage();
		break;

	case Drone::DroneEvent::DATA_UPDATED:
		repaint();
		break;

	default:
		break;
	}
}

void DroneUI::itemDragEnter(const SourceDetails & source)
{
	isDraggingItemOver = true;
	repaint();
}

void DroneUI::itemDragExit(const SourceDetails & source)
{
	isDraggingItemOver = false;
	repaint();
}

void DroneUI::itemDropped(const SourceDetails & source)
{
	MotionBlockModelTreeUI * modelUI = dynamic_cast<MotionBlockModelTreeUI *>(source.sourceComponent.get());

	if (modelUI != nullptr)
	{
		MotionBlockDataProvider * provider = modelUI->model;

		bool shift = KeyPress::isKeyCurrentlyDown(16);
		if (shift)
		{
			PopupMenu m;
			m.addItem(-1, "Default");
			m.addSeparator();
			int index = 1;
			for (auto &p : modelUI->model->presetManager.items) m.addItem(index++, p->niceName);
			int result = m.show();
			if (result >= 1) provider = modelUI->model->presetManager.items[result - 1];
		}

		item->activeProvider->setValueFromTarget(provider);
	}

	isDraggingItemOver = false;
	repaint();
}
