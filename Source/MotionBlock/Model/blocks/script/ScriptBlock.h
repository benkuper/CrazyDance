/*
  ==============================================================================

    ScriptBlock.h
    Created: 10 Apr 2018 6:59:13pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MotionBlockModel.h"

class ScriptBlock :
	public MotionBlockModel,
	public Script::AsyncListener
{
public:
	ScriptBlock(var params = var());
	~ScriptBlock();

	const Identifier updateMotionDataId = "updateMotionData";
	Script script;

	virtual Array<WeakReference<Controllable>> getModelParameters() override;

	void getMotionDataInternal(var result, Drone  * d, double time, int id, var params) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable *) override;
	void childStructureChanged(ControllableContainer * cc) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	
	void newMessage(const Script::ScriptEvent &e) override;

	String getTypeString() const override { return "Script"; }
	static ScriptBlock * create(var params) { return new ScriptBlock(params); }
};