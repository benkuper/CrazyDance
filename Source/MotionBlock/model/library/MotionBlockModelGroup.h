/*
  ==============================================================================

    MotionBlockModelGroup.h
    Created: 13 Apr 2018 2:03:02am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModel.h"

class MotionBlockModelGroup :
	public BaseItem,
	public BaseManager<MotionBlockModelGroup>::ManagerListener,
	public BaseManager<MotionBlockModel>::ManagerListener
{
public:
	MotionBlockModelGroup(const String &name = "Group");
	~MotionBlockModelGroup();
	
	virtual void clear() override;
	
	BaseManager<MotionBlockModelGroup> groupManager;
	BaseManager<MotionBlockModel> modelManager;
	
	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void itemAdded(MotionBlockModel * model) override;
	void itemRemoved(MotionBlockModel * model) override;
	void itemAdded(MotionBlockModelGroup * group) override;
	void itemRemoved(MotionBlockModelGroup * group) override;

	//Listener
	class  ModelGroupListener
	{
	public:
		/** Destructor. */
		virtual ~ModelGroupListener() {}
		virtual void groupModelAdded(MotionBlockModel * /*model*/) {}
		virtual void groupModelRemoved(MotionBlockModel * /*model*/) {}
		virtual void groupModelGroupAdded(MotionBlockModelGroup * /*group*/) {}
		virtual void groupModelGroupRemoved(MotionBlockModelGroup * /*group*/) {}
	};

	ListenerList<ModelGroupListener> groupListeners;
	void addModelGroupListener(ModelGroupListener* newListener) { groupListeners.add(newListener); }
	void removeModelGroupListener(ModelGroupListener* listener) { groupListeners.remove(listener); }


	// ASYNC
	class  ModelGroupEvent
	{
	public:
		enum Type { MODEL_ADDED, MODEL_REMOVED, GROUP_ADDED, GROUP_REMOVED };

		ModelGroupEvent(Type t, MotionBlockModel * m) :
			type(t), model(m), group(nullptr) {}
		ModelGroupEvent(Type t, MotionBlockModelGroup * g) :
			type(t), model(nullptr), group(g) {}

		Type type;
		MotionBlockModel * model;
		MotionBlockModelGroup * group;
	};

	QueuedNotifier<ModelGroupEvent> groupNotifier;
	typedef QueuedNotifier<ModelGroupEvent>::Listener AsyncListener;

	void addAsyncModelGroupListener(AsyncListener* newListener) { groupNotifier.addListener(newListener); }
	void addAsyncCoalescedModelGroupListener(AsyncListener* newListener) { groupNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncModelGroupListener(AsyncListener* listener) { groupNotifier.removeListener(listener); }


	String getTypeString() const override { return "Group"; }

};
