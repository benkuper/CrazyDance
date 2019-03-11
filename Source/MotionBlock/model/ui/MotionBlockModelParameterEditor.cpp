/*
  ==============================================================================

    MotionBlockModelParameterEditor.cpp
    Created: 10 Apr 2018 8:01:56pm
    Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelParameterEditor.h"

MotionBlockModelParameterEditor::MotionBlockModelParameterEditor(MotionBlockModelParameter * a, bool isRoot) :
	BaseItemEditor(a, isRoot),
	arg(a)
{
	editableUI = arg->editable->createToggle();

	addAndMakeVisible(editableUI);

	resetAndBuild();
}

MotionBlockModelParameterEditor::~MotionBlockModelParameterEditor()
{
}

void MotionBlockModelParameterEditor::resetAndBuild()
{
	BaseItemEditor::resetAndBuild();
	for (auto &cui : childEditors)
	{
		ControllableEditor * ce = dynamic_cast<ControllableEditor *>(cui);
		if (ce != nullptr) ce->setShowLabel(false);
	}
}


void MotionBlockModelParameterEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	editableUI->setBounds(r.removeFromRight(70).reduced(2));
	r.removeFromRight(2);
}
