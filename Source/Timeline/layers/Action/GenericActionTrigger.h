/*
  ==============================================================================

    GenericActionTrigger.h
    Created: 7 Mar 2019 7:54:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Action.h"

class GenericActionTrigger :
	public Action
{
public:
	GenericActionTrigger();
	~GenericActionTrigger();

	enum Type { TAKE_OFF, LAND, HEADLIGHT_ON, HEADLIGHT_OFF};
	EnumParameter* actionType ;

	void triggerInternal() override;

	String getTypeString() const override { return "Generic"; }

	static GenericActionTrigger* create(var ) { return new GenericActionTrigger(); }
};