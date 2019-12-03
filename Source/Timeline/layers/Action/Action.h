/*
  ==============================================================================

    Action.h
    Created: 7 Mar 2019 8:07:09pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Drone/Cluster/DroneCluster.h"

class Action :
	public TimeTrigger
{
public:
	Action(const String &name = "Action");
	virtual ~Action();

	DroneCluster cluster;

	virtual var getJSONData() override;
	virtual void loadJSONDataItemInternal(var data) override;

	String getTypeString() const override { return "Action"; }
};
