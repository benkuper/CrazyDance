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

TimelineBlockSequence::TimelineBlockSequence() :
	Sequence()
{

	layerManager->addItem(new MotionBlockLayer(this));
	layerManager->addBaseManagerListener(this);
	setAudioDeviceManager(&AudioManager::getInstance()->am);
}

TimelineBlockSequence::~TimelineBlockSequence()
{
}

var TimelineBlockSequence::getMotionData(Drone * p, double time, var params)
{
	Array<MotionBlockLayer *> layers = getLayersForProp(p);

	int numLayers = layers.size();
	float t = params.getProperty("sequenceTime", true) ? currentTime->floatValue() : time;
	
	int droneCount = -1;

	if (numLayers == 1 && layers[0] != nullptr)
	{
		params.getDynamicObject()->setProperty("forceID", layers[0]->filterManager.getTargetIDForDrone(p, droneCount));
		return layers[0]->getMotionData(p, t, params); //use sequence's time instead of prop time
	}
	
	if (numLayers == 0) return var();

	Vector3D<float> targetMainPos;
	Vector3D<float> effectPos;
	float totalMainWeight = 0;
	int numMainPositions = 0;
	int numEffectPositions = 0;

	for (auto &l : layers)
	{
		if (l == nullptr) continue;
		params.getDynamicObject()->setProperty("forceID", l->filterManager.getTargetIDForDrone(p, droneCount));
		var lResult = l->getMotionData(p, t, params); //use sequence's time instead of prop time
		
		if (!lResult.isObject()) continue;

		var lPosData = lResult.getProperty("position", var());
		float lWeight = lResult.getProperty("weight", 0);

		if (!lPosData.isArray()) continue;

		Vector3D<float> lPos(lPosData[0], lPosData[1], lPosData[2]);
		MotionBlockLayer::Mode m = l->mode->getValueDataAsEnum<MotionBlockLayer::Mode>();

		switch(m)
		{
		case MotionBlockLayer::MAIN:
		{
			targetMainPos += lPos * lWeight;
			totalMainWeight += lWeight;
			numMainPositions++;
		}
		break;

		case MotionBlockLayer::EFFECT:
			effectPos += lPos * lWeight;
			numEffectPositions++;
			break;
		}
	}


	if (numMainPositions == 0 && numEffectPositions == 0) return var();
	if (totalMainWeight > 0 && numMainPositions > 1) targetMainPos /= totalMainWeight;

	Vector3D<float> tPos(targetMainPos + effectPos);

	var result = var(new DynamicObject());
	var posData;
	posData.append(tPos.x);
	posData.append(tPos.y);
	posData.append(tPos.z);
	result.getDynamicObject()->setProperty("position", posData);

	return result;

}

Array<MotionBlockLayer*> TimelineBlockSequence::getLayersForProp(Drone * p, bool includeDisabled)
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

void TimelineBlockSequence::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Sequence::onControllableFeedbackUpdateInternal(cc, c);

}
