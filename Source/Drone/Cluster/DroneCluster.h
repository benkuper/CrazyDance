/*
  ==============================================================================

    DroneCluster.h
    Created: 5 Feb 2019 11:12:24am
    Author:  jonglissimo

  ==============================================================================
*/

#pragma once

#include "../Drone.h"

class DroneCluster :
	public BaseItem
{
public:
	DroneCluster();
	~DroneCluster();

	//ControllableContainer DroneIDsCC;
	Array<int,CriticalSection> droneIDs;

	void updateIDs();

	void onContainerParameterChanged(Parameter *p) override;
	void onControllableAdded(Controllable *) override;
	void onControllableRemoved(Controllable *) override;

	Array<Drone *> getDrones();
	bool isDroneIsInCluster(Drone *) const;
	int getLocalDroneID(Drone *p) const;

	void loadJSONData(var data, bool createIfNotThere = false) override;

};