/*
  ==============================================================================

    MotionBlockClipTransitionUI.cpp
    Created: 13 Mar 2019 11:38:56am
    Author:  bkupe

  ==============================================================================
*/

#include "MotionBlockClipTransitionUI.h"

MotionBlockClipTransitionUI::MotionBlockClipTransitionUI(MotionBlockClipTransition * transition) :
	LayerBlockUI(transition),
	transition(transition)
{
	highlightOnMouseOver = true;
	canBeGrabbed = false;
	repaint();
}

MotionBlockClipTransitionUI::~MotionBlockClipTransitionUI()
{
}

void MotionBlockClipTransitionUI::paint(Graphics & g)
{
	bgColor = (transition->isInverted()?Colours::purple: BLUE_COLOR).withAlpha(.3f);
	LayerBlockUI::paint(g);

	int holdFromRel = transition->holdFrom->floatValue() / transition->coreLength->floatValue();
	int holdToRel= transition->holdTo->floatValue() / transition->coreLength->floatValue();
	int fadeFromRel = transition->fadeFrom->floatValue() / transition->coreLength->floatValue();
	int fadeToRel = transition->fadeTo->floatValue() / transition->coreLength->floatValue();
	
	g.setColour(Colours::orange.withAlpha(.2f));
	if (holdFromRel > 0) g.fillRect(getLocalBounds().removeFromLeft(holdFromRel*getWidth()));
	if (holdToRel > 0) g.fillRect(getLocalBounds().removeFromRight(holdToRel*getWidth()));
	
	g.setColour(Colours::yellow.withAlpha(.2f));
	if (fadeFromRel > 0) g.fillRect(getLocalBounds().removeFromLeft(fadeFromRel*getWidth()));
	if (fadeToRel > 0) g.fillRect(getLocalBounds().removeFromRight(fadeToRel*getWidth()));

	g.setColour(TEXT_COLOR);
	g.drawText(transition->niceName, getLocalBounds().toFloat(), Justification::centred);
}

bool MotionBlockClipTransitionUI::hitTest(int x, int y)
{
	return x > 10 && x < getWidth() - 10;
}
