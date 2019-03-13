/*
  ==============================================================================

	MotionBlockModel.cpp
	Created: 10 Apr 2018 7:12:39pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModel.h"
#include "../MotionBlock.h"
#include "Drone/DroneManager.h"


MotionBlockModel::MotionBlockModel(const String &name, var params, Type type) :
	MotionBlockDataProvider(name, false),
	type(type),
	isBeingEdited(false),
	presetManager(this),
	modelNotifier(5)
{
	itemDataType = "MotionBlockModel";
	paramsContainer = new ControllableContainer("Parameters");
	paramsContainer->saveAndLoadName = false;
	paramsContainer->saveAndLoadRecursiveData = true;
	addChildControllableContainer(paramsContainer);
	addChildControllableContainer(&presetManager);
}

MotionBlockModel::~MotionBlockModel()
{
}

void MotionBlockModel::clear()
{
	presetManager.clear();

	Array<WeakReference<Controllable>> pList = getModelParameters();
	for (auto &p : pList)
	{
		if (p->type != Controllable::TRIGGER) dynamic_cast<Parameter *>(p.get())->resetValue();
	}
}


void MotionBlockModel::setBeingEdited(bool value)
{
	if (value == isBeingEdited) return;
	isBeingEdited = value;
	modelNotifier.addMessage(new ModelEvent(ModelEvent::EDITING_STATE_CHANGED, this));
}

Array<WeakReference<Controllable>> MotionBlockModel::getModelParameters()
{
	if (paramsContainer == nullptr) return Array<WeakReference<Controllable>>();
	return paramsContainer->getAllControllables();
}

var MotionBlockModel::getMotionData(Drone * d, double time, var params)
{
	var result = var(new DynamicObject());
	int id = params.getProperty("forceID", d->globalID->intValue());
	getMotionDataInternal(result, d, time, id, params);
	return result;
}

void MotionBlockModel::setCustomThumbnail(String path)
{
	customThumbnailPath = path;
	modelListeners.call(&ModelListener::customThumbnailChanged, this);
	modelNotifier.addMessage(new ModelEvent(ModelEvent::CUSTOM_THUMBNAIL_CHANGED, this));
}

var MotionBlockModel::getJSONData()
{
	var data = MotionBlockDataProvider::getJSONData();
	if (paramsContainer != nullptr)
	{
		var cData = paramsContainer->getJSONData();
		if (cData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("modelParams", cData);
	}
	if (presetManager.items.size() > 0) data.getDynamicObject()->setProperty("presets", presetManager.getJSONData());
	return data;
}

void MotionBlockModel::loadJSONDataInternal(var data)
{
	MotionBlockDataProvider::loadJSONDataInternal(data);
	if (paramsContainer != nullptr) paramsContainer->loadJSONData(data.getProperty("modelParams", var()), paramsContainer->userCanAddControllables);
	presetManager.loadJSONData(data.getProperty("presets", var()));
}

Vector3D<float> MotionBlockModel::getPoint3DValue(Point3DParameter * p, var params)
{
	var val = getParamValue<var>(p, params);
	if (!val.isArray()) return Vector3D<float>();
	return Vector3D<float>(val[0], val[1], val[2]);
}
int MotionBlockModel::getDroneCount(var params)
{
	return params.getProperty("droneCount", DroneManager::getInstance()->items.size());
}
var MotionBlockModel::getVarForPoint3D(Vector3D<float> val)
{
	var result;
	result.append(val.x);
	result.append(val.y);
	result.append(val.z);
	return result;

}
void MotionBlockModel::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (Engine::mainEngine->isClearing) return;
	if (cc == paramsContainer) providerListeners.call(&ProviderListener::providerParameterValueUpdated, this, dynamic_cast<Parameter *>(c));
}

void MotionBlockModel::childStructureChanged(ControllableContainer * cc)
{
	if (cc == paramsContainer) providerListeners.call(&ProviderListener::providerParametersChanged, this);
}
