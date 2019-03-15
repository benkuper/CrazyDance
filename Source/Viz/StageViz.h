/*
  ==============================================================================

    StageViz.h
    Created: 10 Mar 2019 7:10:29pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Drone/DroneManager.h"
#include "DroneVizUI.h"

class StageViz :
	public BaseManagerShapeShifterViewUI<DroneManager, Drone, DroneVizUI>,
	public DroneVizUI::AsyncListener
{
public:
	StageViz(StringRef name, DroneManager * manager);
	~StageViz();

	ScopedPointer<ControllableUI> lpsBoxUI;

	void paint(Graphics &g) override;

	void updateViewUIPosition(DroneVizUI * dui) override;
	
	void addItemUIInternal(DroneVizUI * dui) override;
	void removeItemUIInternal(DroneVizUI * dui) override;

	void newMessage(const DroneVizUI::VizEvent &e) override;

	static StageViz * create(const String &name) { return new StageViz(name, DroneManager::getInstance()); }
};