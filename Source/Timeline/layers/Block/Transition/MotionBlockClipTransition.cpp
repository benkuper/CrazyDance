/*
  ==============================================================================

	MotionBlockClipTransition.cpp
	Created: 13 Mar 2019 11:38:33am
	Author:  bkupe

  ==============================================================================
*/

#include "MotionBlockClipTransition.h"
#include "../MotionBlockLayer.h"

MotionBlockClipTransition::MotionBlockClipTransition(MotionBlockLayer * layer, WeakReference<MotionBlockClip> fromClip, WeakReference<MotionBlockClip> toClip) :
	LayerBlock("Transition", 0),
	layer(layer),
	fromClip(nullptr),
	toClip(nullptr),
	curve("Curve")
{
	setFromClip(fromClip);
	setToClip(toClip);


	fadeFrom = addFloatParameter("Fade From", "", 0, 0);
	fadeFrom->defaultUI = FloatParameter::TIME;
	fadeTo = addFloatParameter("Fade To", "", 0, 0);
	fadeTo->defaultUI = FloatParameter::TIME;

	holdFrom = addFloatParameter("Hold from", "", 0, 0);
	holdFrom->defaultUI = FloatParameter::TIME;
	holdTo = addFloatParameter("Hold to", "", 0, 0);
	holdTo->defaultUI = FloatParameter::TIME;

	curve.allowKeysOutside = false;
	addChildControllableContainer(&curve);
	curve.addItem(0, 0);
	curve.addItem(1, 1);
	curve.items[0]->setEasing(Easing::BEZIER);
	curve.showUIInEditor = true;
}

MotionBlockClipTransition::~MotionBlockClipTransition()
{
	clearItem();
}

void MotionBlockClipTransition::clearItem()
{
	setFromClip(nullptr);
	setToClip(nullptr);
}

void MotionBlockClipTransition::setFromClip(MotionBlockClip * clip)
{
	if (clip == fromClip) return;

	if (fromClip != nullptr && !fromClip.wasObjectDeleted())
	{
		fromClip->removeControllableContainerListener(this);
	}

	fromClip = clip;
	jassert(fromClip != toClip);

	if (fromClip != nullptr)
	{
		fromClip->addControllableContainerListener(this);
	}

	updateName();
	resyncTimings();
}

void MotionBlockClipTransition::setToClip(MotionBlockClip * clip)
{
	if (clip == toClip) return;

	if (toClip != nullptr && !toClip.wasObjectDeleted())
	{
		toClip->removeControllableContainerListener(this);
	}

	toClip = clip;

	if (toClip != nullptr)
	{
		toClip->addControllableContainerListener(this);

	}

	updateName();
	resyncTimings();
}

bool MotionBlockClipTransition::isInverted()
{
	if (fromClip == nullptr || toClip == nullptr) return false;
	return fromClip->getEndTime() > toClip->time->floatValue();
}

void MotionBlockClipTransition::updateName()
{
	if (fromClip != nullptr && toClip != nullptr) setNiceName(fromClip->niceName + " > " + toClip->niceName);
}

void MotionBlockClipTransition::resyncTimings()
{
	if (fromClip == nullptr || toClip == nullptr) return;
	jassert(fromClip != toClip);

	float fromClipEndTime = fromClip->getEndTime();
	float diff = toClip->time->floatValue() - fromClip->getEndTime();
	float length = fabsf(diff);
	setStartTime(jmin(fromClipEndTime + diff, fromClipEndTime));
	setCoreLength(length, false, false);
}

var MotionBlockClipTransition::getMotionData(Drone * d, double absoluteTime, var params)
{
	if (fromClip == nullptr || toClip == nullptr || fromClip.wasObjectDeleted() || toClip.wasObjectDeleted()) return var();

	float fromTime = fromClip->getEndTime();
	float toTime = toClip->time->floatValue();

	bool inverted = isInverted();

	float targetFromTime = inverted ? absoluteTime : jmin<float>(absoluteTime, fromTime + fadeFrom->floatValue());
	float targetToTime = inverted ? absoluteTime : jmax<float>(absoluteTime, toTime - fadeTo->floatValue());

	DBG("From time : " << targetFromTime << " / " << absoluteTime << " / " << (fromTime + fadeFrom->floatValue()));
	var fromMotionPosData = fromClip->getMotionData(d, targetFromTime, params).getProperty("position", var());
	var toMotionPosData = toClip->getMotionData(d, targetToTime, params).getProperty("position", var());

	if (fromTime == toTime) return fromMotionPosData;
	if (!fromMotionPosData.isArray() || !toMotionPosData.isArray()) return var();

	

	Vector3D<float> fromPos = Vector3D<float>(fromMotionPosData[0], fromMotionPosData[1], fromMotionPosData[2]);
	Vector3D<float> toPos = Vector3D<float>(toMotionPosData[0], toMotionPosData[1], toMotionPosData[2]);

	
	float relTime = jmap<float>(absoluteTime, fromTime+holdFrom->floatValue(), toTime-holdTo->floatValue(), 0, 1);
	relTime = jlimit<float>(0, 1, relTime);

	if (fromTime > toTime) relTime = 1 - relTime; //invert

	Vector3D<float> targetPos = fromPos + (toPos - fromPos) * curve.getValueForPosition(relTime);
	var result = new DynamicObject();
	result.getDynamicObject()->setProperty("position", getVarForPoint3D(targetPos));
	return result;
}


var MotionBlockClipTransition::getVarForPoint3D(Vector3D<float> val)
{
	var result;
	result.append(val.x);
	result.append(val.y);
	result.append(val.z);
	return result;

}

var MotionBlockClipTransition::getJSONData()
{
	var data = LayerBlock::getJSONData();
	if (fromClip != nullptr) data.getDynamicObject()->setProperty("fromClip", fromClip->shortName);
	if (toClip != nullptr) data.getDynamicObject()->setProperty("toClip", toClip->shortName);
	data.getDynamicObject()->setProperty("curve", curve.getJSONData());
	return data;
}

void MotionBlockClipTransition::loadJSONDataInternal(var data)
{
	LayerBlock::loadJSONDataInternal(data);
	setFromClip((MotionBlockClip *)layer->blockClipManager.getItemWithName(data.getProperty("fromClip", "")));
	setToClip((MotionBlockClip *)layer->blockClipManager.getItemWithName(data.getProperty("toClip", "")));
	curve.loadJSONData(data.getProperty("curve", var()));
}

void MotionBlockClipTransition::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	MotionBlockClip * clip = dynamic_cast<MotionBlockClip *>(cc);
	if (clip != nullptr)
	{
		if (c == clip->time || c == clip->coreLength || c == clip->loopLength)
		{
			resyncTimings();
		}
	}
	else
	{
		LayerBlock::onControllableFeedbackUpdateInternal(cc, c);
	}
}
