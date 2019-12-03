/*
  ==============================================================================

    DroneClusterGroup.cpp
    Created: 5 Feb 2019 11:12:42am
    Author:  jonglissimo

  ==============================================================================
*/

#include "DroneClusterGroup.h"

DroneClusterGroup::DroneClusterGroup() :
	BaseItem("Cluster Group"),
	clusterManager("Clusters")
{
	sendFeedback = addBoolParameter("Send Feedback", "If checked, Drone changes will be sent using their local ID", false);

	//clusterManager.hideEditorHeader = true;
	clusterManager.selectItemWhenCreated = false;
	clusterManager.editorCanBeCollapsed = false;

	clusterManager.addItem(); //add one item here

	addChildControllableContainer(&clusterManager);
}

DroneClusterGroup::~DroneClusterGroup()
  {
  }

DroneCluster * DroneClusterGroup::getClusterForDrone(Drone * p, int &localID)
{
	for (auto &c : clusterManager.items)
	{
		int id = c->getLocalDroneID(p);
		if (id >= 0)
		{
			localID = id;
			return c;
		}
	}

	return nullptr;
}

int DroneClusterGroup::getLocalDroneID(Drone * p)
{
	for (auto &c : clusterManager.items)
	{
		int id = c->getLocalDroneID(p);
		if (id >= 0) return id;
	}

	return -1;
}

var DroneClusterGroup::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("clusterManager", clusterManager.getJSONData());
	return data;
}

void DroneClusterGroup::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	clusterManager.loadJSONData(data.getProperty("clusterManager", var()));
}
