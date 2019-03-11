/*
  ==============================================================================

    ScriptBlock.cpp
    Created: 10 Apr 2018 6:59:13pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptBlock.h"
#include "Drone/Drone.h"

ScriptBlock::ScriptBlock(var params) :
	MotionBlockModel(getTypeString(), params),
	script(this)
{
	removeChildControllableContainer(paramsContainer);
	removeChildControllableContainer(&presetManager);
	addChildControllableContainer(&script);
	addChildControllableContainer(&presetManager);

	script.addAsyncScriptListener(this);

	//colorDataObject = colorData.getScriptObject();
}

ScriptBlock::~ScriptBlock()
{
}

Array<WeakReference<Controllable>> ScriptBlock::getModelParameters()
{
	return script.scriptParamsContainer.getAllControllables();
}

void ScriptBlock::getMotionDataInternal(var result, Drone * d, double time, int id, var params)
{
	if (!params.hasProperty("droneCount")) params.getDynamicObject()->setProperty("droneCount", getDroneCount(params));
	Array<var> args;
	args.add(result);
	args.add(id);
	args.add(time);
	args.add(params);

	script.callFunction(updateMotionDataId, args); 
}

void ScriptBlock::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (cc == &script && script.state == Script::SCRIPT_LOADED) providerListeners.call(&ProviderListener::providerParameterValueUpdated, this, dynamic_cast<Parameter *>(c));
}

void ScriptBlock::childStructureChanged(ControllableContainer * cc)
{
	if (cc == &script && script.state == Script::SCRIPT_LOADED) providerListeners.call(&ProviderListener::providerParametersChanged, this);
}

var ScriptBlock::getJSONData()
{
	var data = MotionBlockModel::getJSONData();
	data.getDynamicObject()->setProperty("script", script.getJSONData());
	return data;
}

void ScriptBlock::loadJSONDataInternal(var data)
{
	script.loadJSONData(data.getProperty("script", var()));
	setCustomThumbnail(script.filePath->getFile().withFileExtension("png").getFullPathName());
	MotionBlockModel::loadJSONDataInternal(data);

}

void ScriptBlock::newMessage(const Script::ScriptEvent & e)
{
	switch (e.type)
	{
	case Script::ScriptEvent::STATE_CHANGE:
		setCustomThumbnail(script.filePath->getFile().withFileExtension("png").getFullPathName());
		setNiceName(script.niceName);

		if(script.state == Script::SCRIPT_LOADED) providerListeners.call(&ProviderListener::providerParametersChanged, this);
		break;
	}
}