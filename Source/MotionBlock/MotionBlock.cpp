/*
  ==============================================================================

	MotionBlock.cpp
	Created: 10 Apr 2018 6:56:47pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlock.h"

#include "MotionBlock.h"
#include "Drone/Drone.h"

MotionBlock::MotionBlock(MotionBlockDataProvider * provider) :
	BaseItem(provider->niceName, false),
	provider(provider),
	paramsContainer("Parameters"),
	paramsLoadData(var())
    //automationsManager(&paramsContainer)
{
	addChildControllableContainer(&paramsContainer);

	userCanRemove = false;

	rebuildArgsFromModel();
	provider->addColorProviderListener(this);

	//addChildControllableContainer(&automationsManager);
}

MotionBlock::~MotionBlock()
{
	if (!provider.wasObjectDeleted()) provider->removeColorProviderListener(this);
}

var MotionBlock::getMotionData(Drone * p, double time, var params)
{
	var localParams = params.isVoid()?new DynamicObject():new DynamicObject(*params.getDynamicObject());
	Array<WeakReference<Parameter>> paramList = paramsContainer.getAllParameters();
	

	if (localParams.getProperty("updateAutomation", true))
	{
		for (auto &param : paramList)
		{
			if (param.wasObjectDeleted()) continue;
			if (param->controlMode != Parameter::AUTOMATION  || param->automation == nullptr) continue;
			param->automation->currentTime->setValue(fmodf(time, param->automation->automation.length->floatValue()));
		}
	} else
	{
		for (auto &param : paramList)
		{
			if (param->controlMode != Parameter::AUTOMATION) continue;
			Automation * a = &param->automation->automation;

			float value = a->getValueForPosition(fmodf(time, a->length->floatValue()));
			float normValue = jmap<float>(value, param->minimumValue, param->maximumValue);
			localParams.getDynamicObject()->setProperty(param->shortName,normValue);
		}
	}

	for (auto &param : paramList)
	{
		if (param.wasObjectDeleted() || param == nullptr) continue;
		if(!localParams.hasProperty(param->shortName)) localParams.getDynamicObject()->setProperty(param->shortName, param->getValue());
	}

	if (provider.wasObjectDeleted())
	{
		var result;
		return result;
	}

	return provider->getMotionData(p, time, localParams);
}

void MotionBlock::rebuildArgsFromModel()
{
	if(paramsContainer.controllables.size() > 0) paramsLoadData = paramsContainer.getJSONData();
	//var aData = automationsManager.getJSONData();

	paramsContainer.clear();
	//automationsManager.clear();

	Array<WeakReference<Controllable>> params = provider->getModelParameters();

	for (auto &sc : params)
	{
		if (sc->type == Controllable::TRIGGER)
		{
			 paramsContainer.addTrigger(sc->niceName, sc->description);
		} else
		{
			Parameter * p = ControllableFactory::createParameterFrom(sc, true, true);
			p->setControllableFeedbackOnly(sc->isControllableFeedbackOnly);
			p->addParameterListener(this);
			paramsContainer.addParameter(p);
		}
	}

	paramsContainer.hideInEditor = paramsContainer.controllables.size() == 0;
	paramsContainer.loadJSONData(paramsLoadData);

	//automationsManager.hideInEditor = paramsContainer.hideInEditor;
	//automationsManager.loadJSONData(aData);

}


void MotionBlock::providerParametersChanged(MotionBlockDataProvider *)
{
	rebuildArgsFromModel(); 
}

void MotionBlock::providerParameterValueUpdated(MotionBlockDataProvider *, Parameter * p)
{
	
	if (p == nullptr) return;
	Parameter * tp = paramsContainer.getParameterByName(p->shortName);
	if (tp == nullptr) return;
    
    //set the default value even if overriden, so when doing a manual "reset value", it's back to the actual current one.
    tp->defaultValue = p->getValue();
    
	if (tp->isOverriden) return;
	if (tp->controlMode != Parameter::MANUAL) return;
	
	tp->resetValue();
}

void MotionBlock::parameterControlModeChanged(Parameter * p)
{
	if (p->controlMode == Parameter::AUTOMATION)
	{
		p->automation->mode->setValueWithData(PlayableParameterAutomation::MANUAL);
		//p->automation->hideInEditor = true;
	}

	blockListeners.call(&MotionBlockListener::blockParamControlModeChanged, p);
}

var MotionBlock::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", paramsContainer.getJSONData());
	//data.getDynamicObject()->setProperty("automations", automationsManager.getJSONData());
	return data;

}

void MotionBlock::loadJSONDataInternal(var data)
{
	
	BaseItem::loadJSONDataInternal(data);

	var pData = data.getProperty("params", var());
	paramsContainer.loadJSONData(pData);
	//automationsManager.loadJSONData(data.getProperty("automations", var()));

	if (paramsContainer.controllables.size() == 0) paramsLoadData = pData; //if params where not already there when loading (using script for exemple), store data to use later
}
