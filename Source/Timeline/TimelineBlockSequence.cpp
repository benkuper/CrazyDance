/*
  ==============================================================================

	TimelineBlockSequence.cpp
	Created: 17 Apr 2018 8:30:13pm
	Author:  Ben

  ==============================================================================
*/

#include "TimelineBlockSequence.h"
#include "layers/Block/MotionBlockLayer.h"
#include "Audio/AudioManager.h"
#include "Drone/Drone.h"
#include "Drone/DroneManager.h"

TimelineBlockSequence::TimelineBlockSequence() :
	Sequence()
{
	totalTime->setValue(120); //2 min by default
	layerManager->addItem(new MotionBlockLayer(this));
	layerManager->addBaseManagerListener(this);
	setAudioDeviceManager(&AudioManager::getInstance()->am);

	alwaysSendWhenPlaying = addBoolParameter("Always send when playing", "", false);
	sendTakeOffOnPlay = addBoolParameter("Send Takeoff on play", "", false);
	sendLandOnStop = addBoolParameter("Send Land on stop", "", false);
	sendLandOnPause = addBoolParameter("Send Land on pause", "", false);
	sendLandOnFinish = addBoolParameter("Send Land on finish", "", false);
}

TimelineBlockSequence::~TimelineBlockSequence()
{
}

var TimelineBlockSequence::getMotionData(Drone * p, double time, var params)
{
	MotionBlockLayer * primaryLayer = getPrimaryLayerForDrone(p);
	Array<MotionBlockLayer *> secondaryLayers = getSecondaryLayersForDrone(p);

	int numLayers = secondaryLayers.size();

	if (primaryLayer == nullptr && numLayers == 0) return var();

	float t = params.getProperty("sequenceTime", true) ? currentTime->floatValue() : time;

	int droneCount = -1;

	Vector3D<float> targetPrimaryPos;
	if (primaryLayer != nullptr)
	{
		var pResult = primaryLayer->getMotionData(p, t, params); //use sequence's time instead of prop time
		var primaryPos = pResult.getProperty("position", var());
		if (primaryPos.isArray()) targetPrimaryPos = Vector3D<float>(primaryPos[0], primaryPos[1], primaryPos[2]);
	}


	Vector3D<float> secondaryPos;

	for (auto &l : secondaryLayers)
	{
		if (l == nullptr) continue;
		params.getDynamicObject()->setProperty("forceID", l->filterManager.getTargetIDForDrone(p, droneCount));
		var lResult = l->getMotionData(p, t, params); //use sequence's time instead of prop time

		if (!lResult.isObject()) continue;

		float lWeight = lResult.getProperty("weight", 0);
		var lPosData = lResult.getProperty("position", var());
		if (!lPosData.isArray()) continue;

		Vector3D<float> lPos(lPosData[0], lPosData[1], lPosData[2]);
		secondaryPos += lPos * lWeight;
	}

	Vector3D<float> tPos(targetPrimaryPos + secondaryPos);

	var result = var(new DynamicObject());
	var posData;
	posData.append(tPos.x);
	posData.append(tPos.y);
	posData.append(tPos.z);
	result.getDynamicObject()->setProperty("position", posData);

	if (alwaysSendWhenPlaying->boolValue() && isPlaying->boolValue()) result.getDynamicObject()->setProperty("forceUpdate", true);

	return result;

}

MotionBlockLayer* TimelineBlockSequence::getPrimaryLayerForDrone(Drone * p, bool includeDisabled)
{
	for (auto &i : layerManager->items)
	{
		if (!includeDisabled && !i->enabled->boolValue()) continue;
		MotionBlockLayer * l = dynamic_cast<MotionBlockLayer *>(i);
		if (l == nullptr) continue;

		int droneCount = -1;
		if (l->filterManager.getTargetIDForDrone(p, droneCount) >= 0) return l;
	}

	return nullptr;
}

Array<MotionBlockLayer*> TimelineBlockSequence::getSecondaryLayersForDrone(Drone * p, bool includeDisabled)
{

	if (layerManager == nullptr) return nullptr;
	if (Engine::mainEngine->isClearing) return nullptr;

	//Array<MotionBlockLayer *> defaultLayers;
	Array<MotionBlockLayer *> result;

	for (auto &i : layerManager->items)
	{
		if (!includeDisabled && !i->enabled->boolValue()) continue;
		MotionBlockLayer * l = dynamic_cast<MotionBlockLayer *>(i);
		if (l == nullptr) continue;
		if (l->mode->getValueDataAsEnum<MotionBlockLayer::Mode>() != MotionBlockLayer::SECONDARY) continue;

		int droneCount = -1;
		if (l->filterManager.getTargetIDForDrone(p, droneCount) >= 0) result.add(l);
		//if (l->defaultLayer->boolValue()) defaultLayers.add(l);
	}

	return result;// result.size() > 0 ? result : defaultLayers;
}

void TimelineBlockSequence::itemAdded(SequenceLayer * s)
{

	MotionBlockLayer * l = dynamic_cast<MotionBlockLayer *>(s);
	if (l != nullptr)
	{
		//		if (!Engine::mainEngine->isLoadingFile && layerManager->items.size() == 1) l->defaultLayer->setValue(true);
		return;
	}

	AudioLayer * al = dynamic_cast<AudioLayer *>(s);
	if (al != nullptr)
	{
		al->setAudioProcessorGraph(&AudioManager::getInstance()->graph, AUDIO_OUTPUT_GRAPH_ID);
	}

}

void TimelineBlockSequence::onContainerTriggerTriggered(Trigger * t)
{
	Sequence::onContainerTriggerTriggered(t);

	if (t == playTrigger && sendTakeOffOnPlay->boolValue())
	{
		for (auto &d : DroneManager::getInstance()->items)
		{
			d->takeOff->trigger();
		}
	}
	else if ((t == stopTrigger && sendLandOnStop->boolValue()) || (t == pauseTrigger && sendLandOnPause->boolValue()) || (t == finishTrigger && sendLandOnFinish->boolValue()))
	{
		for (auto &d : DroneManager::getInstance()->items)
		{
			d->land->trigger();
		}
	}
}

void TimelineBlockSequence::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Sequence::onControllableFeedbackUpdateInternal(cc, c);

}
