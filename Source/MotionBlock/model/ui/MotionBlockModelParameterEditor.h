/*
  ==============================================================================

    MotionBlockModelParameterEditor.h
    Created: 10 Apr 2018 8:01:56pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModelParameter.h"

class MotionBlockModelParameterEditor :
	public BaseItemEditor
{
public:
	MotionBlockModelParameterEditor(MotionBlockModelParameter * a, bool isRoot);
	~MotionBlockModelParameterEditor();

	MotionBlockModelParameter * arg;

	ScopedPointer<BoolToggleUI> editableUI;

	void resetAndBuild() override;
	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;

};
