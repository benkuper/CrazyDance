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

	Rectangle<int> r = getLocalBounds();
	

	g.setColour(Colours::yellow.withAlpha(.3f));
	if (transition->fadeFrom->floatValue() > 0) g.fillRect(r.removeFromLeft(transition->fadeFrom->floatValue()*getWidth()));
	if (transition->fadeTo->floatValue() > 0) g.fillRect(r.removeFromRight(transition->fadeTo->floatValue()*getWidth()));


	g.setColour(Colours::orange.withAlpha(.3f));
	if (transition->holdFrom->floatValue() > 0) g.fillRect(r.removeFromLeft(transition->holdFrom->floatValue()*getWidth()));
	if (transition->holdTo->floatValue() > 0) g.fillRect(r.removeFromRight(transition->holdTo->floatValue()*getWidth()));
	
	g.setColour(TEXT_COLOR);
	g.drawText(transition->niceName, getLocalBounds().toFloat(), Justification::centred);
}

bool MotionBlockClipTransitionUI::hitTest(int x, int y)
{
	return x > 10 && x < getWidth() - 10;
}

void MotionBlockClipTransitionUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	LayerBlockUI::controllableFeedbackUpdateInternal(c);

	if (c == transition->holdFrom || c == transition->holdTo || c == transition->fadeFrom || c == transition->fadeTo) repaint();
}
