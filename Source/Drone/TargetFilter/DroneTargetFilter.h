/*
  ==============================================================================

    DroneTargetFilter.h
    Created: 5 Feb 2019 11:13:17am
    Author:  jonglissimo

  ==============================================================================
*/

#pragma once
#include "Drone/Drone.h"

class DroneTargetFilter :
	public BaseItem
{
public:
	DroneTargetFilter(StringRef name = "Drone Target");
	virtual ~DroneTargetFilter();

	bool isDroneValid(Drone * p);
	virtual int getTargetIDForDrone(Drone * p, int &droneCount);
};


class DroneFilterGlobalID :
	DroneTargetFilter
{
public:
	DroneFilterGlobalID();
	~DroneFilterGlobalID();

	IntParameter * id;

	int getTargetIDForDrone(Drone * p, int &droneCount) override;

	String getTypeString() const override { return "Global ID"; }
	static DroneTargetFilter * create(var) { return new DroneFilterGlobalID(); }
};

class DroneFilterCluster :
	DroneTargetFilter
{
public:
	DroneFilterCluster();
	~DroneFilterCluster();
	 
	BoolParameter * specificClusterGroup;
	TargetParameter * clusterGroup;
	BoolParameter * specificCluster;
	TargetParameter * cluster;
	BoolParameter * specificID;
	IntParameter * id;

	int getTargetIDForDrone(Drone * p, int &droneCount) override;

	void onContainerParameterChangedInternal(Parameter * p) override;

	static bool targetIsCluster(ControllableContainer * cc);

	String getTypeString() const override { return "Cluster"; }
	static DroneTargetFilter * create(var) { return new DroneFilterCluster(); }
};


class DroneFilterScript :
	DroneTargetFilter
{
public:
	DroneFilterScript();
	~DroneFilterScript();

	Script script;

	int getTargetIDForDrone(Drone * p, int &droneCount) override;

	String getTypeString() const override { return "Script"; }
	static DroneTargetFilter * create(var) { return new DroneFilterScript(); }
};