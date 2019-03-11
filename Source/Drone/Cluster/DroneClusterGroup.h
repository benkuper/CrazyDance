/*
  ==============================================================================

    DroneClusterGroup.h
    Created: 5 Feb 2019 11:12:42am
    Author:  jonglissimo

  ==============================================================================
*/

#pragma once

#include "DroneCluster.h"

class DroneClusterGroup :
	public BaseItem
{
public:
	DroneClusterGroup();
	~DroneClusterGroup();

	BoolParameter * sendFeedback;

	BaseManager<DroneCluster> clusterManager;

	DroneCluster * getClusterForDrone(Drone * p, int &localID);
	int getLocalDroneID(Drone * p);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
};