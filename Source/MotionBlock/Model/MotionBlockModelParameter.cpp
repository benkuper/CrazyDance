/*
  ==============================================================================

    MotionBlockModelParameter.cpp
    Created: 10 Apr 2018 7:49:54pm
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelParameter.h"
#include "ui/MotionBlockModelParameterEditor.h"


MotionBlockModelParameter::MotionBlockModelParameter(const String &name, Parameter * _p) :
	BaseItem(name, false),
	param(_p)
{
	editorCanBeCollapsed = false;


	isSelectable = false;
	param->isCustomizableByUser = true;
	param->saveValueOnly = false;

	jassert(param != nullptr);
	addControllable(param);

	editable = addBoolParameter("Editable", "If uncheck, argument will always be used at its default value, and not visible in the command editor", true);
	editable->hideInEditor = true;
}

InspectableEditor * MotionBlockModelParameter::getEditor(bool isRoot)
{
	return new MotionBlockModelParameterEditor(this, isRoot);
}

var MotionBlockModelParameter::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("param", param->getJSONData());
	return data;
}

void MotionBlockModelParameter::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	param->loadJSONData(data.getProperty("param", var()));
}

void MotionBlockModelParameter::onContainerNiceNameChanged()
{
	param->setNiceName(niceName);
}
