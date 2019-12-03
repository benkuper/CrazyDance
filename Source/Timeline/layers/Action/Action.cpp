/*
  ==============================================================================

    Action.cpp
    Created: 7 Mar 2019 8:07:09pm
    Author:  bkupe

  ==============================================================================
*/

#include "Action.h"

Action::Action(const String& name) :
	TimeTrigger(name)
{
	addChildControllableContainer(&cluster);
}

Action::~Action()
{
}

var Action::getJSONData()
{
	var data = TimeTrigger::getJSONData();
	data.getDynamicObject()->setProperty("filters", cluster.getJSONData());
	return data;
}

void Action::loadJSONDataItemInternal(var data)
{
	TimeTrigger::loadJSONDataItemInternal(data);
	cluster.loadJSONData(data.getProperty("filters", var()));
}
