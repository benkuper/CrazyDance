/*
  ==============================================================================

    BaseTreeItem.h
    Created: 12 Mar 2019 11:14:33am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

template<class T>
class BaseTreeItem :
	public BaseItem<T>
{
public:
	BaseTreeItem(StringRef name = "", StringRef itemPath = "/") : 
		BaseItem(name), 
		itemPath(itemPath) 
	{
	}

	virtual ~BaseTreeItem()
	{
	}

	String itemPath;

	void setItemPath(StringRef newPath)
	{
		if (newPath == itemPath) return;
		itemPath = newPath;

		itemTreeListeners.call(&ItemTreeListener::itemPathChanged, this);
	}

	String getFullPath() const { return itemPath + "/" + shortName; }

	//Listener
	class  ItemTreeListener
	{
	public:
		/** Destructor. */
		virtual ~ItemTreeListener() {}
		virtual void itemPathChanged(BaseTreeItem *, String previousPath) {}
	};

	ListenerList<ItemTreeListener> itemTreeListeners;
	void addItemTreeListener(ItemTreeListener* newListener) { itemTreeListeners.add(newListener); }
	void removeItemTreelListener(ItemTreeListener* listener) { itemTreeListeners.remove(listener); }
};