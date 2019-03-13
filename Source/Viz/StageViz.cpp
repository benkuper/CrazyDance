/*
  ==============================================================================

    StageViz.cpp
    Created: 10 Mar 2019 7:10:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "StageViz.h"

StageViz::StageViz(StringRef name, DroneManager * manager) :
	BaseManagerShapeShifterViewUI(name, manager)
{
	addExistingItems();
}

StageViz::~StageViz()
{
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
