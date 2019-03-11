/*
  ==============================================================================

	MotionBlockModelGroupUI.cpp
	Created: 10 Apr 2018 11:17:09pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelGroupUI.h"

MotionBlockModelGroupUI::MotionBlockModelGroupUI(ControllableContainer * group) :
	group(group)
{
	for (auto &cc : group->controllableContainers)
	{
		MotionBlockModel * m = dynamic_cast<MotionBlockModel *>(cc.get());
		if (m == nullptr) continue;
		MotionBlockModelUI * mui = m->createUI();
		modelsUI.add(mui);
		addAndMakeVisible(mui);
	}

}

MotionBlockModelGroupUI::~MotionBlockModelGroupUI()
{
}

void MotionBlockModelGroupUI::paint(Graphics & g)
{
	Rectangle<int> hr = getLocalBounds().reduced(2).removeFromTop(headerHeight);
	g.setColour(BG_COLOR.brighter(.1f));
	g.fillRoundedRectangle(hr.toFloat(), 2);
	g.setColour(TEXT_COLOR);
	g.drawFittedText(group->niceName, hr, Justification::centred, 1);
}

void MotionBlockModelGroupUI::resized()
{
	
	Rectangle<int> r = getLocalBounds().reduced(2).translated(0, headerHeight + headerGap);
	if (r.getWidth() == 0) return;

	int numThumbs = modelsUI.size();
	int numThumbPerLine = jmin(r.getWidth() / (thumbSize + gap), numThumbs);
	int numLines = ceil(numThumbs*1.f/ numThumbPerLine);
	
	r.setHeight(numLines * (thumbSize + gap) - gap);

	int index = 0;
	int yIndex = 0;

	Rectangle<int> lr;
	
	for (auto &mui : modelsUI)
	{
		if (index % numThumbPerLine == 0)
		{

			int numThumbsInThisLine = jmin(numThumbs - index, numThumbPerLine);
			int lineWidth = numThumbsInThisLine * (thumbSize + gap) - gap;

			if (yIndex > 0) r.removeFromTop(gap);
			lr = r.removeFromTop(thumbSize);
			lr = lr.withSizeKeepingCentre(lineWidth, lr.getHeight());

			yIndex++;
		}
			
		mui->setBounds(lr.removeFromLeft(thumbSize));
		lr.removeFromLeft(gap);
		index++;
	} 

	setSize(getWidth(),r.getBottom());
}

void MotionBlockModelGroupUI::setThumbSize(int value)
{
	if (value == thumbSize) return;
	thumbSize = value;
	resized();
}
