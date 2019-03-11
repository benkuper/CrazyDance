/*
  ==============================================================================MotionBlockColorProvider

	Drone.h
	Created: 10 Apr 2018 6:59:25pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "MotionBlock/MotionBlock.h"

class DroneFamily;

class Drone :
	public BaseItem,
	public Inspectable::InspectableListener,
	public Thread,
	public MotionBlockDataProvider::ProviderListener
{
public:

	Drone(StringRef name = "Drone", StringRef familyName = "", var params = var());
	virtual ~Drone();

	enum State { OFF, DISCONNECTED, CONNECTING, CALIBRATING, READY, TAKEOFF, FLYING, LANDING, WARNING, MAX_STATES};
	const String stateStrings[MAX_STATES]{ "Off", "Disconnected", "Connecting","Connecting","Calibrating", "Ready", "Takeoff", "Flying", " Warning" };

	ControllableContainer generalCC;
	IntParameter * droneID;
	IntParameter * globalID;
	EnumParameter * state;
	FloatParameter * battery;

	ControllableContainer flightCC;
	Point3DParameter * position;

	ControllableContainer lightCC;
	ColorParameter * color;
	BoolParameter * headlight;

	var motionData;
	ScopedPointer<MotionBlock> currentBlock;
	TargetParameter * activeProvider;

	int previousID; //for swapping
	int updateRate;

	virtual void clearItem() override;

	void setBlockFromProvider(MotionBlockDataProvider * model);

	void update();

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable *c) override;
	void inspectableDestroyed(Inspectable *) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	//Listener
	class  DroneListener
	{
	public:
		/** Destructor. */
		virtual ~DroneListener() {}
		virtual void droneBlockChanged(Drone * /*Drone*/) {}
		virtual void droneGlobalIDChanged(Drone *, int) {}
		virtual void dataUpdated(Drone *) {}
	};

	ListenerList<DroneListener> droneListeners;
	void addDroneListener(DroneListener* newListener) { droneListeners.add(newListener); }
	void removeDroneListener(DroneListener* listener) { droneListeners.remove(listener); }

	// ASYNC
	class  DroneEvent
	{
	public:
		enum Type { BLOCK_CHANGED, DATA_UPDATED };

		DroneEvent(Type t, Drone * p, var v = var()) :
			type(t), Drone(p), value(v) {}

		Type type;
		Drone * Drone;
		var value;
	};

	QueuedNotifier<DroneEvent> droneNotifier;
	typedef QueuedNotifier<DroneEvent>::Listener AsyncListener;

	void addAsyncDroneListener(AsyncListener* newListener) { droneNotifier.addListener(newListener); }
	void addAsyncCoalescedDroneListener(AsyncListener* newListener) { droneNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncDroneListener(AsyncListener* listener) { droneNotifier.removeListener(listener); }

	// Inherited via Thread
	virtual void run() override;
};