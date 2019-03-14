/*
  ==============================================================================

    MotionBlockClip.cpp
    Created: 17 Apr 2018 5:10:24pm
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockClip.h"
#include "MotionBlock/model/library/MotionBlockModelLibrary.h"
#include "MotionBlock/model/blocks/timeline/TimelineBlock.h"

MotionBlockClip::MotionBlockClip(MotionBlockLayer * layer, float _time) :
	LayerBlock("MotionBlockClip", _time),
	layer(layer)
{
	itemDataType = "MotionBlockClip";

	activeProvider = addTargetParameter("Active Block", "The current active block for this prop");
	activeProvider->targetType = TargetParameter::CONTAINER;
	activeProvider->hideInEditor = true;
	
	//activeProvider->customGetTargetContainerFunc = &MotionBlockModelLibrary::showProvidersAndGet;

	autoFade = addBoolParameter("Auto Fade", "If checked, when clips are overlapping, fade will be adjusted automatically", true);
	fadeIn = addFloatParameter("Fade In", "Fade in time", 0, 0, getTotalLength());
	fadeOut = addFloatParameter("Fade Out", "Fade out time", 0, 0, getTotalLength());
	fadeIn->setControllableFeedbackOnly(autoFade->boolValue());
	fadeOut->setControllableFeedbackOnly(autoFade->boolValue());

}

MotionBlockClip::~MotionBlockClip()
{
	setBlockFromProvider(nullptr);
	masterReference.clear();
}


void MotionBlockClip::setBlockFromProvider(MotionBlockDataProvider * provider)
{
	if (currentBlock == nullptr && provider == nullptr) return;

	if (provider != nullptr && currentBlock != nullptr && currentBlock->provider == provider) return;

	if (currentBlock != nullptr)
	{
		blockData = currentBlock->getJSONData();
		removeChildControllableContainer(currentBlock);
		currentBlock->removeMotionBlockListener(this);
		currentBlock = nullptr;
	}

	if (provider != nullptr) currentBlock = new MotionBlock(provider);


	if (currentBlock != nullptr)
	{
		addChildControllableContainer(currentBlock);
		currentBlock->addMotionBlockListener(this);
		//if(parentContainer != nullptr) setNiceName(parentContainer->getUniqueNameInContainer(currentBlock->niceName));
		
		if (!blockData.isVoid())
		{
			currentBlock->loadJSONDataInternal(blockData);
			blockData = var();
		}
	}
}
var MotionBlockClip::getMotionData(Drone * p, double absoluteTime, var params)
{
	var result;
	if (currentBlock == nullptr) return result;
	
	float factor = 1;

	double relTimeTotal = absoluteTime - time->floatValue();
	if (fadeIn->floatValue() > 0) factor *= jmin<double>(relTimeTotal / fadeIn->floatValue(),1.f);
	if (fadeOut->floatValue() > 0) factor *= jmin<double>((getTotalLength() - relTimeTotal) / fadeOut->floatValue(), 1.f);


	if (dynamic_cast<TimelineBlock *>(currentBlock->provider.get()) != nullptr)
	{
		params.getDynamicObject()->setProperty("sequenceTime", false);
	}

	double relTime = getRelativeTime(absoluteTime, true, absoluteTime >= getEndTime()); //if outside of clip, don't loop
	result = currentBlock->getMotionData(p, relTime, params);
	if (result.isObject()) result.getDynamicObject()->setProperty("weight", factor);
	return result;
}

void MotionBlockClip::blockParamControlModeChanged(Parameter * p) 
{
	if (p->controlMode == Parameter::AUTOMATION)
	{
		p->automation->automation.setLength(coreLength->floatValue());
		p->automation->automation.allowKeysOutside = true;
	}
}


void MotionBlockClip::setCoreLength(float value, bool stretch, bool stickToCoreEnd)
{
	LayerBlock::setCoreLength(value, stretch, stickToCoreEnd);

	if (currentBlock != nullptr)
	{
		Array<WeakReference<Parameter>> params = currentBlock->paramsContainer.getAllParameters();
		for (auto & pa : params)
		{
			if (pa->controlMode == Parameter::AUTOMATION) pa->automation->automation.setLength(coreLength->floatValue(), stretch, stickToCoreEnd);
		}
	}
}

void MotionBlockClip::onContainerParameterChangedInternal(Parameter * p)
{
	LayerBlock::onContainerParameterChangedInternal(p);

	if (p == activeProvider)
	{
		setBlockFromProvider(dynamic_cast<MotionBlockDataProvider *>(activeProvider->targetContainer.get()));
	}
	else if (p == coreLength || p == loopLength)
	{
		fadeIn->setRange(0, getTotalLength());
		fadeOut->setRange(0, getTotalLength());
	}
	else if (p == autoFade)
	{
		fadeIn->setControllableFeedbackOnly(autoFade->boolValue());
		fadeOut->setControllableFeedbackOnly(autoFade->boolValue());
	}
}

var MotionBlockClip::getJSONData()
{
	var data = LayerBlock::getJSONData();
	if (currentBlock != nullptr) data.getDynamicObject()->setProperty("blockData", currentBlock->getJSONData());
	return data;
}

void MotionBlockClip::loadJSONDataInternal(var data)
{
	LayerBlock::loadJSONDataInternal(data);

	var bData = data.getProperty("blockData", var());
	if (currentBlock != nullptr)
	{
		currentBlock->loadJSONData(bData);

		Array<WeakReference<Parameter>> params = currentBlock->paramsContainer.getAllParameters();

		for (auto & pa : params)
		{
			if (pa->controlMode == Parameter::AUTOMATION)
			{
				pa->automation->automation.allowKeysOutside = true;
			}
		}
	}
	else
	{
		blockData = bData;
	}


}
