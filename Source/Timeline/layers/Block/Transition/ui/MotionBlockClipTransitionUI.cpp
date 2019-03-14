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
	//LayerBlockUI::paint(g);

	
	
	if (transition->isInverted())
	{
		g.setColour((transition->isInverted() ? Colours::purple : BLUE_COLOR).withAlpha(.3f));
		g.fillRect(getMainBounds());
	}
	else
	{
		Path p;
		p.startNewSubPath(0, 0);
		p.quadraticTo(getWidth() / 2, getHeight() / 3, getWidth(), 0);
		p.lineTo(getWidth(), getHeight());
		p.quadraticTo(getWidth() / 2, 2 * getHeight() / 3, 0, getHeight());
		p.closeSubPath();

		float fadeFrom = transition->fadeFrom->floatValue();
		float fadeTo = transition->fadeTo->floatValue();
		
		g.setColour((inspectable->isSelected?HIGHLIGHT_COLOR:BLUE_COLOR).brighter(isMouseOver() ? .5f:0));
		g.fillPath(p);

		Colour fadeFromC = YELLOW_COLOR;
		Colour fadeToC = YELLOW_COLOR;

		if (fadeFrom > 0)
		{
			ColourGradient gradient = ColourGradient::horizontal(fadeFromC, fadeFromC.withAlpha(0.f), getMainBounds().removeFromLeft(fadeFrom*getWidth()));
			g.setGradientFill(gradient);
			g.fillPath(p);
		}

		if (fadeTo > 0)
		{
			ColourGradient gradient = ColourGradient::horizontal(fadeToC.withAlpha(0.f), fadeToC, getMainBounds().removeFromRight(fadeTo*getWidth()));
			g.setGradientFill(gradient);
			g.fillPath(p);
		}
	}
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
