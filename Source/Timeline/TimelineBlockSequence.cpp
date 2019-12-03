/*
  ==============================================================================

	TimelineBlockSequence.cpp
	Created: 17 Apr 2018 8:30:13pm
	Author:  Ben

  ==============================================================================
*/

#include "TimelineBlockSequence.h"
#include "layers/Color/ColorLayer.h"
#include "layers/Action/ActionLayer.h"
#include "Audio/AudioManager.h"
#include "Drone/Drone.h"
#include "Drone/DroneManager.h"

TimelineBlockSequence::TimelineBlockSequence() :
	Sequence()
{
	totalTime->setValue(120); //2 min by default
	layerManager->addItem(new MotionBlockLayer(this));
	layerManager->addBaseManagerListener(this);

	alwaysSendWhenPlaying = addBoolParameter("Always send when playing", "", false);
	sendTakeOffOnPlay = addBoolParameter("Send Takeoff on play", "", false);
	sendLandOnStop = addBoolParameter("Send Land on stop", "", false);
	sendLandOnPause = addBoolParameter("Send Land on pause", "", false);
	sendLandOnFinish = addBoolParameter("Send Land on finish", "", false);

	//Timeline
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Motion", &MotionBlockLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", ColorLayer::getTypeStringStatic(), &ColorLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Actions", &ActionLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &AudioLayer::create, this));

	setAudioDeviceManager(&AudioManager::getInstance()->am);
}

TimelineBlockSequence::~TimelineBlockSequence()
{
}

var TimelineBlockSequence::getMotionData(Drone * d, double time, var params)
{
	Array<MotionBlockLayer *> primaryLayers = getLayersForDrone(d, MotionBlockLayer::PRIMARY);
	Array<MotionBlockLayer *> overrideLayers = getLayersForDrone(d, MotionBlockLayer::OVERRIDE);
	Array<MotionBlockLayer *> secondaryLayers = getLayersForDrone(d, MotionBlockLayer::SECONDARY);

	if (primaryLayers.isEmpty() && secondaryLayers.isEmpty()) return var();

	float t = params.getProperty("sequenceTime", true) ? currentTime->floatValue() : time;

	int droneCount = -1;

	Vector3D<float> primaryPos;
	
	for (auto & l : primaryLayers)
	{
		var pResult = l->getMotionData(d, t, params).getProperty("position", var()); //use sequence's time instead of prop time
		if (pResult.isArray())
		{
			primaryPos = Vector3D<float>(pResult[0], pResult[1], pResult[2]);
			break;
		}
	}

	Vector3D<float> overridePos;
	float overrideWeight = 0;
	for (auto & l : overrideLayers)
	{
		var clipData = l->getMotionData(d, t, params);
		var posData = clipData.getProperty("position", var()); //use sequence's time instead of prop time
		if (posData.isArray())
		{
			overridePos = Vector3D<float>(posData[0], posData[1], posData[2]);
			
			overrideWeight = clipData.getProperty("weight", 0);
			if (d->globalID->intValue())
			{
				DBG("Drone 0, " << overridePos.x << " > " << overrideWeight);
			}
			break;
		}
	}

	Vector3D<float> targetPrimaryPos = primaryPos + (overridePos - primaryPos) * overrideWeight;

	Vector3D<float> secondaryPos;

	for (auto &l : secondaryLayers)
	{
		if (l == nullptr) continue;
		params.getDynamicObject()->setProperty("forceID", l->filterManager.getTargetIDForDrone(d, droneCount));
		var lResult = l->getMotionData(d, t, params); //use sequence's time instead of prop time

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

	Array<ColorLayer *> colorLayers = getColorLayersForDrone(d);
	if(colorLayers.size() > 0)
	{
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 0;
		for (auto& cl : colorLayers)
		{
			Colour c = cl->getColorAtTime(t);
			r = jmin<float>(r + c.getRed(), 255);
			g = jmin<float>(g + c.getGreen(), 255);
			b = jmin<float>(b + c.getBlue(), 255);
			a = jmin<float>(a + c.getAlpha(), 255);

		}

		var colData;
		colData.append(r);
		colData.append(g);
		colData.append(b);
		colData.append(a);
		result.getDynamicObject()->setProperty("color", colData);
	}
	

	if (alwaysSendWhenPlaying->boolValue() && isPlaying->boolValue()) result.getDynamicObject()->setProperty("forceUpdate", true);

	return result;

}

Array<MotionBlockLayer *> TimelineBlockSequence::getLayersForDrone(Drone * d, MotionBlockLayer::Mode m, bool includeDisabled)
{
	Array<MotionBlockLayer *> result; 
	for (auto &i: layerManager->items)
	{
		if (!includeDisabled && !i->enabled->boolValue()) continue;
		MotionBlockLayer * l = dynamic_cast<MotionBlockLayer *>(i);

		if (l == nullptr) continue;
		if (l->mode->getValueDataAsEnum<MotionBlockLayer::Mode>() != m) continue;
		
		int droneCount = -1;
		if (l->filterManager.getTargetIDForDrone(d, droneCount) >= 0) result.add(l);
	}

	return result;
}

Array<ColorLayer*> TimelineBlockSequence::getColorLayersForDrone(Drone* p, bool includeDisabled)
{
	Array<ColorLayer*> result;
	for (auto &i : layerManager->items)
	{
		if (!includeDisabled && !i->enabled->boolValue()) continue;
		ColorLayer * l = dynamic_cast<ColorLayer *>(i);
		if (l == nullptr) continue;

		int droneCount = -1;
		if (l->filterManager.getTargetIDForDrone(p, droneCount) >= 0) result.add(l);
	}

	return result;
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
