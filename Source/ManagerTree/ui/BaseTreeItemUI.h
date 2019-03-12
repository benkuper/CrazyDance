/*
  ==============================================================================

    BaseTreeItemUI.h
    Created: 12 Mar 2019 11:14:50am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../BaseTreeItem.h"

template<class T, class C>
class BaseTreeItemUI :
	public TreeViewItem
{
public:
	BaseTreeItemUI(T* item) :
		TreeViewItem()
	{

	}

	WeakReference<BaseTreeItem> treeItem;

	Component * createItemComponent() override
	{
		return new C(item);
	}
	
	virtual int getItemHeight() const override { return 20; }
	String getUniqueName() const override { return static_cast<BaseTreeItem *>(item)->getFullPath(); }
	virtual bool mightContainSubItems() override { return false; }

	virtual ~BaseTreeItemUI() {}
};

template<class T>
class BaseTreeItemComponentUI :
	public InspectableContentComponent
{
public:
	BaseTreeItemComponentUI(BaseItemTree<T> * t) :
		InspectableContentComponent(t),
		item(t)
	{

	}

	~BaseTreeItemComponentUI()
	{

	}

	BaseTreeItem * item;

	void paint(Graphics &g) override
	{
		if (inspectable.wasObjectDeleted()) return;

		if (inspectable->isSelected && autoDrawContourWhenSelected)
		{
			g.setColour(HIGHLIGHT_COLOR);
			g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
		}

		g.setColour(TEXT_COLOR);
		g.setFont(getHeight() - 6);
		g.drawFittedText(item->niceName)
	}

	void inspectableSelectionChanged(Inspectable *) override;
	{
		ifrepaint(autoDrawContourWhenSelected) repaint();
	}
};