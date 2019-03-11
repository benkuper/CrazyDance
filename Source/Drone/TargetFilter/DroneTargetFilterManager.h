/*
  ==============================================================================

    DroneTargetFilterManager.h
    Created: 5 Feb 2019 12:43:58pm
    Author:  jonglissimo

  ==============================================================================
*/

#pragma once

#include "DroneTargetFilter.h"

class DroneTargetFilterManager :
	public BaseManager<DroneTargetFilter>
{
public:
	DroneTargetFilterManager();
	~DroneTargetFilterManager();

	Factory<DroneTargetFilter> factory;

	int getTargetIDForDrone(Drone *, int &droneCount);

	void addItemInternal(DroneTargetFilter *, var data) override;
	void removeItemInternal(DroneTargetFilter *) override;

	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable *c) override;

	class  FilterManagerListener
	{
	public:
		/** Destructor. */
		virtual ~FilterManagerListener() {}
		virtual void filtersChanged() {}
	};

	ListenerList<FilterManagerListener> filerManagerListeners;
	void addFilterManagerListener(FilterManagerListener * newListener) { filerManagerListeners.add(newListener); }
	void removeFilterManagerListener(FilterManagerListener* listener) { filerManagerListeners.remove(listener); }

};