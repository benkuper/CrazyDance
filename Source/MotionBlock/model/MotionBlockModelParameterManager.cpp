/*
  ==============================================================================

    MotionBlockModelParameterManager.cpp
    Created: 10 Apr 2018 7:49:47pm
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelParameterManager.h"
#include "ui/MotionBlockModelParameterManagerEditor.h"

MotionBlockModelParameterManager::MotionBlockModelParameterManager() :
	BaseManager("arguments")
{
	selectItemWhenCreated = false;
	userCanAddItemsManually = false;
}

MotionBlockModelParameter * MotionBlockModelParameterManager::addItemWithParam(Parameter * p, var data, bool fromUndoableAction)
{
	MotionBlockModelParameter * a = new MotionBlockModelParameter("#" + String(items.size() + 1), p);
	return addItem(a, data, fromUndoableAction);
}

MotionBlockModelParameter * MotionBlockModelParameterManager::addItemFromType(Parameter::Type type, var data, bool fromUndoableAction)
{
	Parameter * p = nullptr;
	String id = String(items.size() + 1);

	switch (type)
	{
	case Parameter::STRING:
		p = new StringParameter("#" + id, "Parameter #" + id + ", type int", "myString");
		break;
	case Parameter::FLOAT:
		p = new FloatParameter("#" + id, "Parameter #" + id + ", type foat", 0, 0, 1);
		break;
	case Parameter::INT:
		p = new IntParameter("#" + id, "Parameter #" + id + ", type int", 0, -1000, 1000);
		break;
	case Parameter::BOOL:
		p = new BoolParameter("#" + id, "Parameter #" + id + ", type bool", false);
		break;
	case Parameter::COLOR:
		p = new ColorParameter("#" + id, "Parameter #" + id + ", type color", Colours::black);
		break; 
	default:
		break;
	}

	if (p == nullptr) return nullptr;
	return addItemWithParam(p, data, fromUndoableAction);
}

MotionBlockModelParameter * MotionBlockModelParameterManager::addItemFromData(var data, bool fromUndoableAction)
{
	String s = data.getProperty("type", "");
	if (s.isEmpty()) return nullptr;
	Parameter * p = (Parameter *)ControllableFactory::createControllable(s);
	if (p == nullptr)
	{
		LOG("Problem loading OSC Command model argument");
		return nullptr;
	}

	return addItemWithParam(p, data, fromUndoableAction);
}

void MotionBlockModelParameterManager::autoRenameItems()
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->niceName.startsWithChar('#')) items[i]->setNiceName("#" + String(i + 1));
	}
}

void MotionBlockModelParameterManager::removeItemInternal(MotionBlockModelParameter *)
{
	autoRenameItems();
}

InspectableEditor * MotionBlockModelParameterManager::getEditor(bool isRoot)
{
	return new MotionBlockModelParameterManagerEditor(this, isRoot);
}