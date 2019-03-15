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
	setSize(100,100);
}

DroneVizUI::~DroneVizUI()
{
}

void DroneVizUI::paint(Graphics & g)
{

	Rectangle<int> r = getMainBounds();

	g.setColour(item->color->getColor());
	g.drawEllipse(getLocalBounds().toFloat(),3);

	if (droneImage.getWidth() > +0) g.drawImage(droneImage, r.toFloat(), RectanglePlacement::centred);

	
	g.setFont(10);
	Colour c = item->uiColor->getColor();
	g.setColour(c.withAlpha(.5f)); 
	g.fillRoundedRectangle(r.withSizeKeepingCentre(g.getCurrentFont().getStringWidth(item->niceName) , g.getCurrentFont().getHeight()).expanded(4,4).toFloat(), 2);
	g.setColour(c.getPerceivedBrightness() < .5f?c.brighter(1):c.darker(1));
	g.drawText(item->niceName, getMainBounds(), Justification::centred);
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
	else if (c == item->position || c == item->color)
	{
		int targetSize = jmap<float>(item->position->y, 0, 20, 5*viewZoom,20*viewZoom);
		if (targetSize != getWidth()) setSize(targetSize, targetSize);

		repaint();
		vizNotifier.addMessage(new VizEvent(VizEvent::POSITION_UPDATED, this));
	}
	else if (c == item->color) vizNotifier.addMessage(new VizEvent(VizEvent::COLOR_UPDATED, this));
	else if (c == item->headlight) vizNotifier.addMessage(new VizEvent(VizEvent::HEADLIGHT_UPDATED, this));
	
}
