/*
  ==============================================================================

    MotionBlockModel.h
    Created: 10 Apr 2018 7:12:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlockModelParameterManager.h"
#include "preset/MotionBlockModelPresetManager.h"
#include "MotionBlockDataProvider.h"

class MotionBlock;
class MotionBlockModelUI;
class MotionBlockModelTreeUI;

class MotionBlockModel :
	public MotionBlockDataProvider
{
public:	
	enum Type { PATTERN, TIMELINE, SCRIPT, NOT_SET };
		
	MotionBlockModel(const String &name = "MotionBlockModel", var params = var(), Type type = NOT_SET);
	~MotionBlockModel();

	Type type;
	
	//ui
	String customThumbnailPath;

	bool isBeingEdited;
	void setBeingEdited(bool value);

	void clear() override;

	ScopedPointer<ControllableContainer> paramsContainer;
	MotionBlockModelPresetManager presetManager;

	virtual Array<WeakReference<Controllable>> getModelParameters() override;

	virtual var getMotionData(Drone * p, double time, var params) override;
	virtual void getMotionDataInternal(var result, Drone * p, double time, int id, var params) {}

	template<class T>
	T getParamValue(Parameter * p, var params);
	Vector3D<float> getPoint3DValue(Point3DParameter * p, var params);
	int getDroneCount(var params);
	var getVarForPoint3D(Vector3D<float> val);

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
	void childStructureChanged(ControllableContainer * cc) override;

	//ui
	void setCustomThumbnail(String path);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	
	virtual MotionBlockModelUI * createUI();

	//Listener
	class  ModelListener
	{
	public:
		/** Destructor. */
		virtual ~ModelListener() {}
		virtual void customThumbnailChanged(MotionBlockModel * /*model*/) {}
	};

	ListenerList<ModelListener> modelListeners;
	void addModelListener(ModelListener* newListener) { modelListeners.add(newListener); }
	void removeModelListener(ModelListener* listener) { modelListeners.remove(listener); }


	// ASYNC
	class  ModelEvent
	{
	public:
		enum Type { CUSTOM_THUMBNAIL_CHANGED, EDITING_STATE_CHANGED};

		ModelEvent(Type t, MotionBlockModel * m) :
			type(t), model(m) {}

		Type type;
		MotionBlockModel * model;
	};

	QueuedNotifier<ModelEvent> modelNotifier;
	typedef QueuedNotifier<ModelEvent>::Listener AsyncListener;

	void addAsyncModelListener(AsyncListener* newListener) { modelNotifier.addListener(newListener); }
	void addAsyncCoalescedModelListener(AsyncListener* newListener) { modelNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncModelListener(AsyncListener* listener) { modelNotifier.removeListener(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MotionBlockModel)
};

template<class T>
T MotionBlockModel::getParamValue(Parameter * p, var params)
{
	return (T)params.getProperty(p->shortName, p->getValue());
}
