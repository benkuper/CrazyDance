/*
  ==============================================================================

    BaseManagerTree.h
    Created: 12 Mar 2019 11:15:26am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "BaseTreeItem.h"

template<class T>
class BaseManagerTree :
	public BaseManager<T>,
	public BaseTreeItem<T>::ItemTreeListener
{
public:
	BaseManagerTree(StringRef name) : BaseManager(name) {}
	virtual ~BaseManagerTree() {}

	HashMap<String, T *> pathToItemMap;
	ValueTree itemTree;


	virtual void addItemInternal(T * item, var params) override
	{
		pathToItemMap.set(static_cast<BaseItem *>(item)->itemPath, item);
	}

	virtual void removeItemInternal(T * item) override
	{
		pathToItemMap.remove(static_cast<BaseItem *>(item)->itemPath);
	}

	T * getItemWithPath(StringRef path)
	{
		return pathToItemMap.contains(path) ? pathToItemMap[path] : nullptr;
	}


	virtual void itemPathChanged(T * item) override
	{
		pathToItemMap.removeValue(item);
		pathToItemMap.set(static_cast<BaseItem *>(item)->itemPath, item)

		managerTreeListeners.call(&ManagerTreeListener::itemPathChanged, item);
		managerTreeNotifier.addMessage(new BManagerTreeEvent(BManagerTreeEvent::ITEM_PATH_CHANGED, item));

		itemPathChangedInternal(item);
	}

	virtual void itemPathChangedInternal(T * item) {}

	class  ManagerTreeListener
	{
	public:
		/** Destructor. */
		virtual ~ManagerTreeListener() {}
		virtual void itemPathChanged(T *) {}
	};

	ListenerList<ManagerTreeListener> managerTreeListeners;
	void addManagerTreeListener(Listener* newListener) { managerTreeListeners.add(newListener); }
	void removeManagerTreeListener(Listener* listener) { managerTreeListeners.remove(listener); }

	class ManagerTreeEvent
	{
	public:
		enum Type { ITEM_PATH_CHANGED };

		ManagerTreeEvent(Type t, T * i = nullptr) : type(t) { itemsRef.add(i); }
		ManagerTreeEvent(Type t, Array<T *> iList) : type(t)
		{
			for (auto &i : iList) itemsRef.add(dynamic_cast<Inspectable *>(i));
		}

		Type type;
		Array<WeakReference<Inspectable>> itemsRef;

		Array<T *> getItems() const
		{
			Array<T *> result;
			for (auto &i : itemsRef) if (i != nullptr && !i.wasObjectDeleted()) result.add(dynamic_cast<T *>(i.get()));
			return result;
		}

		T * getItem(int index = 0) const
		{
			if (itemsRef.size() > index && itemsRef[index] != nullptr && !itemsRef[index].wasObjectDeleted()) return static_cast<T *>(itemsRef[index].get());
			return nullptr;
		}
	};

	using BManagerTreeEvent = typename BaseManagerTree<T>::ManagerTreeEvent;
	using ManagerNotifier = QueuedNotifier<BManagerTreeEvent>;
	ManagerNotifier managerTreeNotifier;
	typedef typename QueuedNotifier<BManagerTreeEvent>::Listener AsyncListener;

	void addAsyncManagerTreeListener(AsyncListener * newListener) { managerTreeNotifier.addListener(newListener); }
	void addAsyncCoalescedManagerTreeListener(AsyncListener * newListener) { managerTreeNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncManagerTreeListener(AsyncListener * listener) { managerTreeNotifier.removeListener(listener); }
};
