/*
  ==============================================================================

	MotionBlockLayer.cpp
	Created: 17 Apr 2018 5:10:36pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockLayer.h"
#include "ui/MotionBlockLayerPanel.h"
#include "ui/MotionBlockLayerTimeline.h"
#include "Drone/DroneManager.h"

MotionBlockLayer::MotionBlockLayer(Sequence * s, var) :
	SequenceLayer(s, "Block Layer"),
	blockClipManager(this),
	transitionManager(this)
{
	mode = addEnumParameter("Mode", "Mode for this layer.\nAll \"Main\" layers will be averaged together, then the \"Effect\" layers will be added to that.");
	mode->addOption("Primary", PRIMARY)->addOption("Secondary", SECONDARY)->addOption("Override", OVERRIDE);

	prePostMode = addEnumParameter("PrePost Mode", "Mode for this layer.\nAll \"Main\" layers will be averaged together, then the \"Effect\" layers will be added to that.");
	prePostMode->addOption("None", NONE)->addOption("Hold", HOLD);// ->addOption("Loop", LOOP)->addOption("Mirror", MIRROR);

	//defaultLayer = addBoolParameter("Default", "If checked, this layer will be the default layer when no layer has the requested prop id", false);

	
	addChildControllableContainer(&blockClipManager);
	blockClipManager.hideInEditor = true;
	addChildControllableContainer(&transitionManager);
	transitionManager.hideInEditor = true;

	addChildControllableContainer(&filterManager);
	filterManager.addFilterManagerListener(this);

	updateLinkedProps();

	Engine::mainEngine->addEngineListener(this);

	blockClipManager.addBaseManagerListener(this);
}

MotionBlockLayer::~MotionBlockLayer()
{

}

var MotionBlockLayer::getMotionData(Drone * d, double time, var params)
{
	if (blockClipManager.items.size() == 0) return var();
	
	if (mode->getValueDataAsEnum<Mode>() == PRIMARY)
	{
		MotionBlockClip * firstBlock = (MotionBlockClip *)blockClipManager.items[0];
		MotionBlockClip * lastBlock = (MotionBlockClip *)blockClipManager.items[blockClipManager.items.size()-1];

		PrePostMode pm = prePostMode->getValueDataAsEnum<PrePostMode>();
		if (time < firstBlock->time->floatValue())
		{
			if(pm == NONE) return var();
			else if(pm == HOLD) return firstBlock->getMotionData(d, firstBlock->time->floatValue(), params);
		}
		else if (time >= lastBlock->getEndTime())
		{
			if (pm == NONE) return  var();
			else if (pm == HOLD) return lastBlock->getMotionData(d, lastBlock->getEndTime(), params);
		}
	}

	var motionData;
	MotionBlockClipTransition * transition = (MotionBlockClipTransition *)transitionManager.getBlockAtTime(time);
	if (transition != nullptr)
	{
		motionData = transition->getMotionData(d, time, params);
	}
	else
	{
		MotionBlockClip * clip = (MotionBlockClip *)blockClipManager.getBlockAtTime(time);
		if (clip != nullptr)
		{
			motionData = clip->getMotionData(d, time, params);
		}
	}

	return motionData;
}

void MotionBlockLayer::updateLinkedProps()
{
	if (DroneManager::getInstanceWithoutCreating() != nullptr)
	{
		for (auto &p : DroneManager::getInstance()->items)
		{
			int droneCount = 0;
			if (filterManager.getTargetIDForDrone(p, droneCount) >= 0)
			{
				linkedInspectables.addIfNotAlreadyThere(p);
				p->linkedInspectables.addIfNotAlreadyThere(this);
			}
			else
			{
				linkedInspectables.removeAllInstancesOf(p);
				p->linkedInspectables.removeAllInstancesOf(this);
			}
		}
	}
}

void MotionBlockLayer::filtersChanged()
{
	updateLinkedProps();
}

void MotionBlockLayer::endLoadFile()
{
	updateLinkedProps();
	Engine::mainEngine->removeEngineListener(this);
}

SequenceLayerPanel * MotionBlockLayer::getPanel()
{
	return new MotionBlockLayerPanel(this);
}

SequenceLayerTimeline * MotionBlockLayer::getTimelineUI()
{
	return new MotionBlockLayerTimeline(this);
}

void MotionBlockLayer::itemAdded(LayerBlock * b)
{
	if (isCurrentlyLoadingData) return;
	if (blockClipManager.items.size() < 2) return;
	if (mode->getValueDataAsEnum<Mode>() == SECONDARY) return;


	MotionBlockClip * clip = dynamic_cast<MotionBlockClip *>(b);

	MotionBlockClip * prevClip = nullptr;
	MotionBlockClip * nextClip = nullptr;
	blockClipManager.getSurroundingBlocks(clip, prevClip, nextClip);

	MotionBlockClipTransition * beforeTransition = transitionManager.getAfterTransition(prevClip);
	MotionBlockClipTransition * afterTransition = transitionManager.getBeforeTransition(nextClip);


	if (afterTransition == nullptr) 
	{
		if(nextClip != nullptr) transitionManager.addTransition(clip, nextClip);
	}
	
	if (beforeTransition == nullptr) 
	{
		if(prevClip != nullptr) transitionManager.addTransition(prevClip, clip);
	}
	
	if(afterTransition != nullptr && beforeTransition != nullptr)//middle
	{
		if (beforeTransition != afterTransition)
		{
			DBG("Weird overlapping");
			return;
		}

		beforeTransition->setToClip(clip);
		transitionManager.addTransition(clip, nextClip);
	}
	
}

void MotionBlockLayer::itemRemoved(LayerBlock * b)
{
	if (isCurrentlyLoadingData) return;
	if (mode->getValueDataAsEnum<Mode>() == SECONDARY) return;

	MotionBlockClip * clip = dynamic_cast<MotionBlockClip *>(b);

	MotionBlockClipTransition * beforeTransition = nullptr;
	MotionBlockClipTransition * afterTransition = nullptr;
	transitionManager.getTransitionsForClip(clip, beforeTransition, afterTransition);
	//MotionBlockClip * prevClip = beforeTransition != nullptr ? beforeTransition->fromClip : nullptr;
	MotionBlockClip * nextClip = afterTransition != nullptr ? afterTransition->toClip : nullptr;

	if (beforeTransition != nullptr && afterTransition != nullptr) //between
	{
		jassert(nextClip != nullptr);
		transitionManager.removeItem(afterTransition);
		beforeTransition->setToClip(nextClip);
	}else
	{
		if(beforeTransition != nullptr) transitionManager.removeItem(beforeTransition);
		if(afterTransition != nullptr) transitionManager.removeItem(afterTransition);
	}
}

var MotionBlockLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("blocks", blockClipManager.getJSONData());
	data.getDynamicObject()->setProperty("transitions", transitionManager.getJSONData());
	data.getDynamicObject()->setProperty("filters", filterManager.getJSONData());
	return data;
}

void MotionBlockLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	blockClipManager.loadJSONData(data.getProperty("blocks", var()));
	transitionManager.loadJSONData(data.getProperty("transitions", var()));
	filterManager.loadJSONData(data.getProperty("filters", var()));
}
