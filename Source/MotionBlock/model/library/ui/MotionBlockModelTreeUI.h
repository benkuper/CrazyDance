/*
  ==============================================================================

    MotionBlockModelTreeUI.h
    Created: 12 Mar 2019 6:20:35pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../MotionBlockModel.h"

class MotionBlockModelTreeUI :
	public InspectableContentComponent,
	public DragAndDropContainer
{
public:
	MotionBlockModelTreeUI(MotionBlockModel * model);
	~MotionBlockModelTreeUI();

	static const Identifier dragAndDropID;

	
	MotionBlockModel * model;
	void paint(Graphics &g) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;

	bool keyPressed(const KeyPress &e) override;

};


class ModelTreeViewItem :
	public InspectableContentComponent,
	public TreeViewItem,
	public MotionBlockModel::AsyncListener
{
public:
	ModelTreeViewItem(MotionBlockModel * model);
	~ModelTreeViewItem();

	MotionBlockModel * model;

	Component * createItemComponent() override { return new MotionBlockModelTreeUI(model); }
	int getItemHeight() const override { return 16; }
	bool mightContainSubItems() override { return false; }

	void newMessage(const MotionBlockModel::ModelEvent &e) override;
};
