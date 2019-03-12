/*
  ==============================================================================

    BaseManagerTreeUI.h
    Created: 12 Mar 2019 11:15:06am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseManagerTree.h"
#include "BaseTreeItemUI.h"

class BaseItemContainerTreeUI :
	public TreeViewItem
{
public:
	BaseItemContainerTreeUI(StringRef path) : path(path) {}
	~BaseItemContainerTreeUI() {}
	
	String path;

	virtual int getItemHeight() const override { return 20; }
	String getUniqueName() const override { return path; }
	virtual bool mightContainSubItems() override { return true; }
};


template<class T>
class BaseManagerTreeUI :
	public InspectableContentComponent,
	public BaseManagerTree<T>::AsyncListener,
	public BaseManager<T>::AsyncListener
{
public:
	BaseManagerTreeUI(BaseManager<T> * manager) :
		InspectableContentComponent(manager),
		manager(manager),
		rootItem("/")
	{
		setDefaultOpenness(true);
		setRootItem(&ri);
		//setRootItemVisible(false);
		getViewport()->setScrollBarThickness(10);

		rebuildTree();
		manager->addBaseManagerAsyncListener(this);
		manager->addBaseManagerTreeAsyncListener(this);
	}

	~BaseManagertreeUI()
	{
		if (!inspectable.wasObjectDeleted())
		{
			manager->removeBaseManagerAsyncListener(this);
			manager->removeBaseManagerTreeAsyncListener(this);
		}
	}

	TreeView tree;
	BaseItemContainerTreeUI rootItem;

	virtual void rebuildTree()
	{

	}


	virtual void newMessage(const BManagerEvent &e) override;
	{
		switch (e.type)
		{
			cas 
		}
	}

	virtual void newMessage(const BManagerTreeEvent &e) override;
	{

	}
};