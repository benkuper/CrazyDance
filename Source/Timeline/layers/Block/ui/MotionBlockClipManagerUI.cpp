/*
  ==============================================================================

	MotionBlockClipManagerUI.cpp
	Created: 17 Apr 2018 7:20:49pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockClipManagerUI.h"
#include "MotionBlockLayerTimeline.h"
#include "MotionBlock/model/ui/MotionBlockModelTreeUI.h"


MotionBlockClipManagerUI::MotionBlockClipManagerUI(MotionBlockLayerTimeline * _timeline, MotionBlockClipManager * manager) :
	LayerBlockManagerUI(_timeline, manager),
	clipManager(manager),
	timeline(_timeline),
	dropClipX(-1)
{
	addExistingItems();
}

MotionBlockClipManagerUI::~MotionBlockClipManagerUI()
{
}

void MotionBlockClipManagerUI::paint(Graphics & g)
{
	LayerBlockManagerUI::paint(g);
	if (dropClipX >= 0)
	{
		g.fillAll(BLUE_COLOR.withAlpha(.3f));
		g.setColour(HIGHLIGHT_COLOR);
		g.drawLine(dropClipX, 0, dropClipX, getHeight(), 4);
	}
}

LayerBlockUI * MotionBlockClipManagerUI::createUIForItem(LayerBlock * item)
{
	return new MotionBlockClipUI((MotionBlockClip *)item);
}

bool MotionBlockClipManagerUI::isInterestedInDragSource(const SourceDetails & source)
{
	return source.description.getProperty("type", "") == MotionBlockModelTreeUI::dragAndDropID.toString();
}

void MotionBlockClipManagerUI::itemDragEnter(const SourceDetails & source)
{
	dropClipX = source.localPosition.x;
	repaint();
}

void MotionBlockClipManagerUI::itemDragExit(const SourceDetails & source)
{
	dropClipX = -1;
	repaint();
}

void MotionBlockClipManagerUI::itemDragMove(const SourceDetails & source)
{
	dropClipX = source.localPosition.x;
	repaint();
}

void MotionBlockClipManagerUI::itemDropped(const SourceDetails & source)
{
	dropClipX = -1;

	MotionBlockModelTreeUI * modelUI = dynamic_cast<MotionBlockModelTreeUI *>(source.sourceComponent.get());
	MotionBlockClip * clip = (MotionBlockClip *)manager->addBlockAt(timeline->getTimeForX(source.localPosition.x));
	if (modelUI == nullptr || clip == nullptr) return;

	MotionBlockDataProvider * provider = modelUI->model;

	bool shift = KeyPress::isKeyCurrentlyDown(16);
	if (shift)
	{
		PopupMenu m;
		m.addItem(-1, "Default");
		m.addSeparator();
		int index = 1;
		for (auto &p : modelUI->model->presetManager.items) m.addItem(index++, p->niceName);
		int result = m.show();
		if (result >= 1) provider = modelUI->model->presetManager.items[result - 1];
	}

	clip->activeProvider->setValueFromTarget(provider);
}
