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

	Point3DParameter * lpsBox;
	Point3DParameter * globalScale;
	Point3DParameter * globalOffset;

    void addItemInternal(Drone * d, var) override;
	void removeItemInternal(Drone * d) override;

	Drone * getDroneWithId(int id, Drone * exludeDrone = nullptr);
	
	int getFirstAvailableID();
	void droneGlobalIDChanged(Drone * d, int previousID) override;
};
