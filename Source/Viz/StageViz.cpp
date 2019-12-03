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

	Vector3D<float> scale = DroneManager::getInstance()->globalScale->getVector();
	Vector3D<float> offset = DroneManager::getInstance()->globalOffset->getVector();
	if (scale.x != 1 || scale.y != 1 || scale.z != 1 || offset.x != 0 || offset.y != 0 || offset.z != 0)
	{
		g.setColour(HIGHLIGHT_COLOR);
		g.drawText("Scale : " + String(scale.x) + ", " + String(scale.y) + ", " + String(scale.z)+"\n"+
			"Offset : " + String(offset.x) + ", " + String(offset.y) + ", " + String(offset.z) + "\n",
				Rectangle<float>(20,20,100,40), Justification::topLeft);
	}
}

void StageViz::updateViewUIPosition(DroneVizUI * dui)
{
	Vector3D<float> pos = dui->item->position->getVector(); 
	Point<int> vPos = getPosInView(Point<float>(pos.x, -pos.z)*100).toInt();
	
	dui->setCentrePosition(vPos);
	dui->resized();
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
