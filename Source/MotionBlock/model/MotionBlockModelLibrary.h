/*
  ==============================================================================

    MotionBlockModelLibrary.h
    Created: 10 Apr 2018 10:57:18pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "UserMotionBlockModelManager.h"
#include "MotionBlockModelGroup.h"

class MotionBlockModelLibrary :
	public ControllableContainer
{
public:
	juce_DeclareSingleton(MotionBlockModelLibrary, true)
	MotionBlockModelLibrary();
	~MotionBlockModelLibrary();

	MotionBlockModelGroup genericBlocks;
	UserMotionBlockModelManager scriptBlocks;
	UserMotionBlockModelManager timelineBlocks;
	
	//Generic / Patterns
	ScopedPointer<MotionBlockModel> positionBlock;
	ScopedPointer<MotionBlockModel> lineBlock;
	ScopedPointer<MotionBlockModel> circleBlock;
	ScopedPointer<MotionBlockModel> pingPongBlock;
	ScopedPointer<MotionBlockModel> multiPosBlock;

	//Interface
	IntParameter * iconSize;

	void clear() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	Array<MotionBlockModel *> getAllModels(bool includeUserModels = true);
	MotionBlockModel * getModelWithName(const String &modelName);

	static MotionBlockDataProvider * showProvidersAndGet();
	static MotionBlockDataProvider * showAllModelsAndGet(bool includePresets);
	static Array<MotionBlockDataProvider *> fillProvidersMenu(PopupMenu &menu, bool includePresets, int startIndex = 1);
	static Array<MotionBlockDataProvider *> fillUserMotionBlockManagerMenu(UserMotionBlockModelManager * manager, PopupMenu &menu, bool includePresets, int startIndex);

};