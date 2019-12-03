/*
  ==============================================================================

    ActionLayer.cpp
    Created: 7 Mar 2019 7:32:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionLayer.h"
#include "ActionManager.h"

ActionLayer::ActionLayer(Sequence * _sequence, var params) :
	TriggerLayer(_sequence, "Actions", params)
{
	setManager(new ActionManager(this, _sequence));
}

ActionLayer::~ActionLayer()
{
}
