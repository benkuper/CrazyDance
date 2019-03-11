/*
  ==============================================================================

    DroneManager.h
    Created: 10 Mar 2019 8:49:55am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Drone.h"

class DroneManager :
	public BaseManager<Drone>,
	public Drone::DroneListener
{
public:
	juce_DeclareSingleton(DroneManager, true)

	DroneManager();
	~DroneManager();

	void addItemInternal(Drone * d, var);
	void removeItemInternal(Drone * d);

	Drone * getDroneWithId(int id, Drone * exludeDrone = nullptr);
	
	int getFirstAvailableID();
	void droneGlobalIDChanged(Drone * d, int previousID) override;
};
