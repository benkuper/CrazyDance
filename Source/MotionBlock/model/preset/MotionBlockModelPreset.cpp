/*
  ==============================================================================

	MotionBlockModelPreset.cpp
	Created: 12 Apr 2018 1:10:55pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelPreset.h"
#include "../MotionBlockModel.h"

MotionBlockModelPreset::MotionBlockModelPreset(MotionBlockModel * model, var) :
	MotionBlockDataProvider("Preset", false),
	model(model),
	modelRef(model),
	paramsContainer("Parameters")
{
	isSelectable = false;
	model->addControllableContainerListener(this);

	paramsContainer.userCanAddControllables = model->paramsContainer->userCanAddControllables;
	paramsContainer.customUserCreateControllableFunc = model->paramsContainer->customUserCreateControllableFunc;

	paramsContainer.editorCanBeCollapsed = false;
	paramsContainer.hideEditorHeader = !paramsContainer.userCanAddControllables;

	rebuildArgsFromModel();

	addChildControllableContainer(&paramsContainer);
}

MotionBlockModelPreset::~MotionBlockModelPreset()
{
	if (!modelRef.wasObjectDeleted()) model->removeControllableContainerListener(this);
}

var MotionBlockModelPreset::getMotionData(Drone * p, double time, var params, var  * blockMemoryData)
{
	return model->getMotionData(p, time, params, blockMemoryData);
}

void MotionBlockModelPreset::rebuildArgsFromModel(bool syncValues)
{

	Array<WeakReference<Controllable>> params = model->getModelParameters();
	if (params.size() > 0) previousData = paramsContainer.getJSONData();


	if (!paramsContainer.userCanAddControllables) paramsContainer.clear();
	else
	{
		Array<Controllable *> controllablesToRemove;
		for (auto &c : params)
		{
			if (c->isRemovableByUser) controllablesToRemove.add(c);
		}

		for (auto & c : controllablesToRemove) paramsContainer.removeControllable(c);
	}

	for (auto &sc : params)
	{
		if (sc->type == Controllable::TRIGGER)
		{
			paramsContainer.addTrigger(sc->niceName, sc->description);
		}
		else
		{
			Parameter * p = ControllableFactory::createParameterFrom(sc, true, true);
			if (p != nullptr)
			{
				p->forceSaveValue = true;
				p->setControllableFeedbackOnly(sc->isControllableFeedbackOnly);
				paramsContainer.addParameter(p);
			}
		}
	}

	//hideInEditor = controllables.size() == 0;

	paramsContainer.loadJSONData(previousData);
}

void MotionBlockModelPreset::childStructureChanged(ControllableContainer * cc)
{
	if(cc == model) rebuildArgsFromModel();
}

void MotionBlockModelPreset::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (cc == &paramsContainer) providerListeners.call(&ProviderListener::providerParameterValueUpdated, this, dynamic_cast<Parameter *>(c));
}

var MotionBlockModelPreset::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", paramsContainer.getJSONData());
	return data;
}

void MotionBlockModelPreset::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	previousData = data.getProperty("params", var());
	paramsContainer.loadJSONData(previousData, paramsContainer.userCanAddControllables);
}

Array<WeakReference<Controllable>> MotionBlockModelPreset::getModelParameters()
{
	return paramsContainer.getAllControllables();
}
