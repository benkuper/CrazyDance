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
	curve("Curve"),
	fadeCurve("Fade Curve")
{
	setFromClip(fromClip);
	setToClip(toClip);


	fadeFrom = addFloatParameter("Fade From", "", 0, 0, 1);
	fadeTo = addFloatParameter("Fade To", "", 0, 0, 1);

	holdFrom = addFloatParameter("Hold from", "", 0, 0, 1);
	holdTo = addFloatParameter("Hold to", "", 0, 0, 1);

	curve.allowKeysOutside = false;
	addChildControllableContainer(&curve);
	curve.addItem(0, 0);
	curve.addItem(1, 1);
	curve.items[0]->setEasing(Easing::BEZIER);
	curve.showUIInEditor = true;
	CubicEasing * cb = (CubicEasing *)(curve.items[0]->easing.get());
	cb->anchor1->setPoint(.3f,0);
	cb->anchor2->setPoint(.6f,1);

	fadeCurve.allowKeysOutside = false;
	addChildControllableContainer(&fadeCurve);
	fadeCurve.addItem(0, 0);
	fadeCurve.addItem(1, .5f);
	fadeCurve.items[0]->setEasing(Easing::BEZIER);
	CubicEasing * fb = (CubicEasing *)(fadeCurve.items[0]->easing.get());
	fb->anchor1->setPoint(.4f, .8f);
	fb->anchor2->setPoint(.5f, 1);

	fadeCurve.showUIInEditor = true;
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

	float curBlockTime = getRelativeTime(absoluteTime, true, true);

	float fromTime = fromClip->getEndTime();
	float toTime = toClip->time->floatValue();

	bool inverted = isInverted();

	float fadeFromAbs = fadeFrom->floatValue() * coreLength->floatValue();
	float fadeToAbs = fadeTo->floatValue() * coreLength->floatValue();

	float smoothedFadeFrom = fadeFromAbs == 0 ? fromTime : (fromTime + fadeCurve.getValueForPosition(curBlockTime / fadeFromAbs) * fadeFromAbs);
	float smoothedFadeTo = fadeToAbs == 0 ? toTime : (toTime - fadeCurve.getValueForPosition((coreLength->floatValue()-curBlockTime) / fadeToAbs) * fadeToAbs);

	float targetFromTime = inverted ? absoluteTime : jmin<float>(absoluteTime, smoothedFadeFrom);
	float targetToTime = inverted ? absoluteTime : jmax<float>(absoluteTime, smoothedFadeTo);

	var fromMotionPosData = fromClip->getMotionData(d, targetFromTime, params).getProperty("position", var());
	var toMotionPosData = toClip->getMotionData(d, targetToTime, params).getProperty("position", var());

	if (fromTime == toTime) return fromMotionPosData;
	if (!fromMotionPosData.isArray() || !toMotionPosData.isArray()) return var();


	Vector3D<float> fromPos = Vector3D<float>(fromMotionPosData[0], fromMotionPosData[1], fromMotionPosData[2]);
	Vector3D<float> toPos = Vector3D<float>(toMotionPosData[0], toMotionPosData[1], toMotionPosData[2]);
	
	float holdFromRel = holdFrom->floatValue() * coreLength->floatValue();
	float holdToRel = holdTo->floatValue() * coreLength->floatValue();

	float relTime = jmap<float>(absoluteTime, fromTime + holdFromRel, toTime - holdToRel, 0, 1);
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
	data.getDynamicObject()->setProperty("fadeCurve", fadeCurve.getJSONData());
	return data;
}

void MotionBlockClipTransition::loadJSONDataInternal(var data)
{
	LayerBlock::loadJSONDataInternal(data);
	setFromClip((MotionBlockClip *)layer->blockClipManager.getItemWithName(data.getProperty("fromClip", "")));
	setToClip((MotionBlockClip *)layer->blockClipManager.getItemWithName(data.getProperty("toClip", "")));
	//curve.loadJSONData(data.getProperty("curve", var()));
	//fadeCurve.loadJSONData(data.getProperty("fadeCurve", var()));
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
