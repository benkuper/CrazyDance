/*
  ==============================================================================

    StageViz.cpp
    Created: 10 Mar 2019 7:10:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "StageViz.h"
#include "Drone/DroneManager.h"

StageViz::StageViz(StringRef name, DroneManager * manager) :
	BaseManagerShapeShifterViewUI(name, manager)
{
	addExistingItems();

	lpsBoxUI = DroneManager::getInstance()->lpsBox->createDefaultUI();
	addAndMakeVisible(lpsBoxUI);
}

StageViz::~StageViz()
{
}

void StageViz::paint(Graphics & g)
{
	BaseManagerShapeShifterViewUI::paint(g);

	if (DroneManager::getInstanceWithoutCreating() == nullptr) return;
	
	Vector3D<float> box = DroneManager::getInstance()->lpsBox->getVector();
	Rectangle<float> boxPlane(-box.x/2, -box.z/2, box.x, box.z);
	Rectangle<float> boxViewSize = getBoundsInView(boxPlane);

	g.setColour(Colours::white.withAlpha(.3f));
	g.drawRect(boxViewSize, 2);
}

void StageViz::updateViewUIPosition(DroneVizUI * dui)
{
	Vector3D<float> pos = dui->item->position->getVector(); 
	Point<int> vPos = getPosInView(Point<float>(pos.x, -pos.z)).toInt();

	dui->setCentrePosition(vPos);
}

void StageViz::addItemUIInternal(DroneVizUI * dui)
{
	dui->addAsyncDroneVizListener(this);
}

void StageViz::removeItemUIInternal(DroneVizUI * dui)
{
	dui->removeAsyncDroneVizListener(this);
}

void StageViz::newMessage(const DroneVizUI::VizEvent & e)
{
	if (e.type == DroneVizUI::VizEvent::POSITION_UPDATED)
	{
		updateViewUIPosition(e.vizUI);
	}
}
