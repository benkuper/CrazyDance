/*
  ==============================================================================

    MotionBlockModelGroupTreeUI.h
    Created: 12 Mar 2019 6:45:40pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModelGroup.h"
#include "../MotionBlockModel.h"


class MotionBlockModelGroupTreeUI :
	public InspectableContentComponent
{
public:
	MotionBlockModelGroupTreeUI(MotionBlockModelGroup * group);
	~MotionBlockModelGroupTreeUI();

	MotionBlockModelGroup * group;

	void paint(Graphics &g) override;

	void mouseDown(const MouseEvent &e) override;
};

class GroupTreeViewItem :
	public InspectableContentComponent,
	public TreeViewItem,
	public MotionBlockModelGroup::AsyncListener
{
public:
	GroupTreeViewItem(MotionBlockModelGroup * group);
	~GroupTreeViewItem();

	MotionBlockModelGroup * group;
	
	void rebuildTree();

	Component * createItemComponent() override { return new MotionBlockModelGroupTreeUI(group); }
	int getItemHeight() const override { return 16; }
	bool mightContainSubItems() { return group->modelManager.items.size() > 0 || group->groupManager.items.size() > 0; }

	void newMessage(const MotionBlockModelGroup::ModelGroupEvent &e) override;
};