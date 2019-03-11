/*
  ==============================================================================

    DroneVizUI.cpp
    Created: 10 Mar 2019 8:11:12pm
    Author:  bkupe

  ==============================================================================
*/

#include "DroneVizUI.h"

DroneVizUI::DroneVizUI(Drone * d) :
	BaseItemUI(d, Direction::NONE, Direction::NONE),
	vizNotifier(10)
{
	showEnableBT = false;
	showRemoveBT = false;
	itemLabel.setVisible(false);
	updateDroneImage();
	setSize(40,40);
}

DroneVizUI::~DroneVizUI()
{
}

void DroneVizUI::paint(Graphics & g)
{
	if (droneImage.getWidth() > +0) g.drawImage(droneImage, getLocalBounds().toFloat(), RectanglePlacement::centred);
	g.setColour(TEXT_COLOR);
	g.setFont(10);
	g.drawText(item->globalID->stringValue() + ":" + item->droneID->stringValue(), Rectangle<float>(0, 0, getMainBounds().getWidth(), 10), Justification::centred);
}

void DroneVizUI::updateDroneImage()
{
	Drone::State s = item->state->getValueDataAsEnum<Drone::State>();
	String stateString = item->stateStrings[(int)s];
	int numBytes;
	const char * imgData = BinaryData::getNamedResource(("drone_" + stateString.toLowerCase() + "_png").getCharPointer(), numBytes);
	droneImage = ImageCache::getFromMemory(imgData, numBytes);
	repaint();
}

void DroneVizUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->state) updateDroneImage();
	else if (c == item->position) vizNotifier.addMessage(new VizEvent(VizEvent::POSITION_UPDATED, this));
	else if (c == item->color) vizNotifier.addMessage(new VizEvent(VizEvent::COLOR_UPDATED, this));
	else if (c == item->headlight) vizNotifier.addMessage(new VizEvent(VizEvent::HEADLIGHT_UPDATED, this));
	
}
