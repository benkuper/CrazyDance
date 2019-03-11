/*
  ==============================================================================

    MotionBlockModelPresetManager.h
    Created: 12 Apr 2018 1:12:10pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlockModelPreset.h"
class MotionBlockModel;

class MotionBlockModelPresetManager :
	public BaseManager<MotionBlockModelPreset>
{
public:
	MotionBlockModelPresetManager(MotionBlockModel * model);
	~MotionBlockModelPresetManager();

	MotionBlockModel * model;
	MotionBlockModelPreset * createItem() override;
};