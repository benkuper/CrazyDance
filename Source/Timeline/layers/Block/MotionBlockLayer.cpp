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
	mode->addOption("Main", MAIN)->addOption("Effect", EFFECT);

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

var MotionBlockLayer::getMotionData(Drone * p, double time, var params)
{
	var motionData;
	MotionBlockClipTransition * transition = (MotionBlockClipTransition *)transitionManager.getBlockAtTime(time);
	if (transition != nullptr)
	{
		motionData = transition->getMotionData(p, time, params);
	}
	else
	{
		MotionBlockClip * clip = (MotionBlockClip *)blockClipManager.getBlockAtTime(time);
		if (clip != nullptr)
		{
			motionData = clip->getMotionData(p, time, params);
		}
	}

	return motionData;


	/*
	Array<LayerBlock *> blocks = blockClipManager.get(time, false);

	if (blocks.size() == 0) return var();

	Vector3D<float> tPos;
	int numPositions = 0;

	int droneCount = -1;
	filterManager.getTargetIDForDrone(p, droneCount);
	if (droneCount > -1)
	{
		if (params.isVoid()) params = var(new DynamicObject());
		params.getDynamicObject()->setProperty("droneCount", droneCount);
	}
	
	float totalWeight = 0;
	for (auto &b : blocks)
	{
		MotionBlockClip * clip = (MotionBlockClip *)b;
		var r = (clip->getMotionData(p, time, params));
		var rPos = r.getProperty("position", var());
		float weight = (float)r.getProperty("weight", 0);

		if (rPos.isArray())
		{
			tPos += Vector3D<float>(rPos[0], rPos[1], rPos[2]) * weight;
			totalWeight += weight;
			numPositions++;
		}
	}

	//tPos /= numPositions;
	if (totalWeight == 0) return var();

	tPos /= totalWeight;
	float weight = jmin(totalWeight, 1.f);

	
	var result = var(new DynamicObject());
	var posData;
	posData.append(tPos.x);
	posData.append(tPos.y);
	posData.append(tPos.z);
	result.getDynamicObject()->setProperty("position", posData);
	result.getDynamicObject()->setProperty("weight", weight);

	
	return result;
	*/
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

	if (blockClipManager.items.size() == 1) return;

	MotionBlockClip * clip = dynamic_cast<MotionBlockClip *>(b);

	MotionBlockClip * prevClip = nullptr;
	MotionBlockClip * nextClip = nullptr;
	blockClipManager.getSurroundingBlocks(clip, prevClip, nextClip);

	DBG("Add clip " << clip->time->floatValue() << ", " << blockClipManager.items.indexOf(clip));

	if (nextClip != nullptr)
	{
		DBG("Next clip " << nextClip->niceName);
	}

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
