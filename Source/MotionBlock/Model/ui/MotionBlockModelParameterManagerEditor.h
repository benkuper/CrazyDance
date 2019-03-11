/*
  ==============================================================================

    MotionBlockModelParameterManagerEditor.h
    Created: 10 Apr 2018 7:55:28pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModelParameterManager.h"

class MotionBlockModelParameterManagerEditor :
	public GenericManagerEditor<MotionBlockModelParameter>
{
public:
	MotionBlockModelParameterManagerEditor(MotionBlockModelParameterManager * manager, bool isRoot);
	~MotionBlockModelParameterManagerEditor() {};

	MotionBlockModelParameterManager * pManager;

	void showMenuAndAddItem(bool) override;
};

