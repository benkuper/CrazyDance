/*
  ==============================================================================

    DroneUI.h
    Created: 10 Mar 2019 8:50:00am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Drone.h"

class DroneUI :
	public BaseItemUI<Drone>,
	public Drone::AsyncListener,
	public DragAndDropTarget
{
public:
	DroneUI(Drone * d);
	~DroneUI();

	bool isDraggingItemOver;

	Image droneImage;
	Image blockImage;

	void paint(Graphics &g) override;
	void updateDroneImage();
	void updateBlockImage();
	void controllableFeedbackUpdateInternal(Controllable * c) override;

	void newMessage(const Drone::DroneEvent &e) override;

	bool isInterestedInDragSource(const SourceDetails &source) override;
	void itemDragEnter(const SourceDetails &source) override;
	void itemDragExit(const SourceDetails &source) override;
	void itemDropped(const SourceDetails &source) override;
};