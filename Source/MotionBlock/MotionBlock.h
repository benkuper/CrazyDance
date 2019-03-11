/*
  ==============================================================================

	MotionBlock.h
	Created: 10 Apr 2018 6:56:47pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "model/MotionBlockModel.h"

class MotionBlock :
	public BaseItem,
	public MotionBlockDataProvider::ProviderListener
{
public:
	MotionBlock(MotionBlockDataProvider * provider);
	virtual ~MotionBlock();

	WeakReference<MotionBlockDataProvider> provider;
	ControllableContainer paramsContainer;


	var paramsLoadData;

	var getMotionData(Drone * p, double time, var params);

	void rebuildArgsFromModel();

	void providerParametersChanged(MotionBlockDataProvider *) override;
	void providerParameterValueUpdated(MotionBlockDataProvider *, Parameter * p) override;
	virtual void parameterControlModeChanged(Parameter * p) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	
	class  MotionBlockListener
	{
	public:
		virtual ~MotionBlockListener() {}
		virtual void blockParamControlModeChanged(Parameter * p) {}
		//virtual void colorsUpdated() {}
	};

	ListenerList<MotionBlockListener> blockListeners;
	void addMotionBlockListener(MotionBlockListener* newListener) { blockListeners.add(newListener); }
	void removeMotionBlockListener(MotionBlockListener* listener) { blockListeners.remove(listener); }
	


	static MotionBlock * create(MotionBlockDataProvider * provider) { return new MotionBlock(provider); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MotionBlock)
};