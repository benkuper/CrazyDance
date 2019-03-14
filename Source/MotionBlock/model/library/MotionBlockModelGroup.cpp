/*
  ==============================================================================

    MotionBlockModelGroup.cpp
    Created: 13 Apr 2018 2:03:02am
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelGroup.h"
#include "MotionBlockModelLibrary.h"

MotionBlockModelGroup::MotionBlockModelGroup(const String & name) :
	BaseItem(name, false, false),
	groupManager("Groups"),
	modelManager("Models"),
	groupNotifier(10)
{
	groupManager.hideEditorHeader = true;
	modelManager.hideEditorHeader = true;

	addChildControllableContainer(&groupManager);
	addChildControllableContainer(&modelManager);
	
	//groupManager.skipControllableNameInAddress = true;
	//modelManager.skipControllableNameInAddress = true;
	//groupManager.skipLabelInTarget = true;
	//modelManager.skipLabelInTarget = true;

	groupManager.addBaseManagerListener(this);
	modelManager.addBaseManagerListener(this);


}

MotionBlockModelGroup::~MotionBlockModelGroup() 
{

}

void MotionBlockModelGroup::clear()
{
	groupManager.clear();
	modelManager.clear();
}

var MotionBlockModelGroup::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	data.getDynamicObject()->setProperty("groups", groupManager.getJSONData());
	data.getDynamicObject()->setProperty("models", modelManager.getJSONData());
	return data;
}

void MotionBlockModelGroup::loadJSONDataInternal(var data)
{
	if(modelManager.managerFactory == nullptr) modelManager.managerFactory = &MotionBlockModelLibrary::getInstance()->factory;
	groupManager.loadJSONData(data.getProperty("groups", var()));
	modelManager.loadJSONData(data.getProperty("models", var()));
}


void MotionBlockModelGroup::itemAdded(MotionBlockModel * model)
{
	groupListeners.call(&ModelGroupListener::groupModelAdded, model);
	groupNotifier.addMessage(new ModelGroupEvent(ModelGroupEvent::MODEL_ADDED, model));

	askConfirmationBeforeRemove = true;
}
void MotionBlockModelGroup::itemRemoved(MotionBlockModel * model)
{
	groupListeners.call(&ModelGroupListener::groupModelRemoved, model);
	groupNotifier.addMessage(new ModelGroupEvent(ModelGroupEvent::MODEL_ADDED, model));


	askConfirmationBeforeRemove = modelManager.items.size() > 0 || groupManager.items.size() > 0;
}
void MotionBlockModelGroup::itemAdded(MotionBlockModelGroup * group)
{
	groupListeners.call(&ModelGroupListener::groupModelGroupAdded, group);
	groupNotifier.addMessage(new ModelGroupEvent(ModelGroupEvent::GROUP_ADDED, group));

	askConfirmationBeforeRemove = true;
}
void MotionBlockModelGroup::itemRemoved(MotionBlockModelGroup * group)
{
	groupListeners.call(&ModelGroupListener::groupModelGroupRemoved, group);
	groupNotifier.addMessage(new ModelGroupEvent(ModelGroupEvent::GROUP_ADDED, group));

	askConfirmationBeforeRemove = modelManager.items.size() > 0 || groupManager.items.size() > 0;

}
