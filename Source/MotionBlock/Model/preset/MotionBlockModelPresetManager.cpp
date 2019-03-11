/*
  ==============================================================================

	MotionBlockModelPresetManager.cpp
	Created: 12 Apr 2018 1:12:10pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockModelPresetManager.h"

MotionBlockModelPresetManager::MotionBlockModelPresetManager(MotionBlockModel * model) :
	BaseManager("Presets"),
	model(model)
{
}

MotionBlockModelPresetManager::~MotionBlockModelPresetManager()
{
}

MotionBlockModelPreset * MotionBlockModelPresetManager::createItem()
{
	return new MotionBlockModelPreset(model);
}
