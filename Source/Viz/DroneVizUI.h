/*
  ==============================================================================

    DroneVizUI.h
    Created: 10 Mar 2019 8:11:12pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Drone/Drone.h"

class DroneVizUI :
	public BaseItemUI<Drone>
{
public:
	DroneVizUI(Drone * d);
	~DroneVizUI();

	Image droneImage;
	void paint(Graphics &g) override;
	void updateDroneImage();
	void controllableFeedbackUpdateInternal(Controllable * c) override;

	// ASYNC
	class  VizEvent
	{
	public:
		enum Type { POSITION_UPDATED, COLOR_UPDATED, HEADLIGHT_UPDATED };

		VizEvent(Type t, DroneVizUI * p) :
			type(t), vizUI(p) {}

		Type type;
		DroneVizUI * vizUI;
	};

	QueuedNotifier<VizEvent> vizNotifier;
	typedef QueuedNotifier<VizEvent>::Listener AsyncListener;

	void addAsyncDroneVizListener(AsyncListener* newListener) { vizNotifier.addListener(newListener); }
	void addAsyncCoalescedDroneVizListener(AsyncListener* newListener) { vizNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncDroneVizListener(AsyncListener* listener) { vizNotifier.removeListener(listener); }

};