/*
  ==============================================================================

    MotionBlockDataProvider.cpp
    Created: 12 Apr 2018 1:47:03pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class MotionBlock;
class Drone;

class MotionBlockDataProvider : 
	public BaseItem
{
public:
	MotionBlockDataProvider(const String &name = "Provider", bool canBeDisabled = true, bool canHaveScripts = false);
	virtual ~MotionBlockDataProvider();
	
	Trigger * assignToAll;

	virtual Array<WeakReference<Controllable>> getModelParameters() = 0;
	virtual var getMotionData(Drone * p, double time, var params) = 0;

	//Bake info
	enum BakeControl { PLAY, PAUSE, STOP, SEEK };

	void onContainerTriggerTriggered(Trigger *) override;

	class ProviderListener
	{
	public:
		virtual ~ProviderListener() {}
		virtual void providerParametersChanged(MotionBlockDataProvider *) {}
		virtual void providerParameterValueUpdated(MotionBlockDataProvider *, Parameter *) {}
		virtual void providerBakeControlUpdate(BakeControl control, var data = var()) {}
	};

	ListenerList<ProviderListener> providerListeners;
	void addColorProviderListener(ProviderListener* newListener) { providerListeners.add(newListener); }
	void removeColorProviderListener(ProviderListener* listener) { providerListeners.remove(listener); }

private:
	WeakReference<MotionBlockDataProvider>::Master masterReference;
	friend class WeakReference<MotionBlockDataProvider>;
};
