/*
  ==============================================================================

    MotionBlockModelParameterManager.h
    Created: 10 Apr 2018 7:49:47pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlockModelParameter.h"

class MotionBlockModelParameterManager :
	public BaseManager<MotionBlockModelParameter>
{
public:
	MotionBlockModelParameterManager();
	~MotionBlockModelParameterManager() {}

	MotionBlockModelParameter * addItemWithParam(Parameter * p, var data = var(), bool fromUndoableAction = false);
	MotionBlockModelParameter * addItemFromType(Parameter::Type type, var data = var(), bool fromUndoableAction = false);

	MotionBlockModelParameter * addItemFromData(var data, bool fromUndoableAction = false) override;

	void autoRenameItems();
	void removeItemInternal(MotionBlockModelParameter * i) override;
	InspectableEditor * getEditor(bool isRoot) override;

};
