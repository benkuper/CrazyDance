/*
  ==============================================================================

    MotionBlockModelLibrary.h
    Created: 10 Apr 2018 10:57:18pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlockModelGroup.h"

class MotionBlockModelLibrary :
	public MotionBlockModelGroup
{
public:
	juce_DeclareSingleton(MotionBlockModelLibrary, true)
	MotionBlockModelLibrary();
	~MotionBlockModelLibrary();

	//Interface
	IntParameter * iconSize;


	Factory<MotionBlockModel> factory;

	void showMenuAndCreateModelOrGroup(MotionBlockModelGroup * group);

	
	/*
	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	Array<MotionBlockModel *> getAllModels(bool includeUserModels = true);
	MotionBlockModel * getModelWithName(const String &modelName);

	
	static MotionBlockDataProvider * showProvidersAndGet();
	static MotionBlockDataProvider * showAllModelsAndGet(bool includePresets);
	static Array<MotionBlockDataProvider *> fillProvidersMenu(PopupMenu &menu, bool includePresets, int startIndex = 1);
	*/


};