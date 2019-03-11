/*
  ==============================================================================

    MotionBlockModelGroup.cpp
    Created: 13 Apr 2018 2:03:02am
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelGroup.h"

var MotionBlockModelGroup::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	for (auto &cc : controllableContainers)
	{
		var ccData = cc->getJSONData();
		if(ccData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(cc->shortName, ccData);
	}
	return data;
}

void MotionBlockModelGroup::loadJSONDataInternal(var data)
{
	ControllableContainer::loadJSONDataInternal(data);
	for (auto &cc : controllableContainers)
	{
		cc->loadJSONData(data.getProperty(cc->shortName, var()));
	}
}
