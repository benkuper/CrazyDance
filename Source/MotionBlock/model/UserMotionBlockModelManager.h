/*
  ==============================================================================

    UserMotionBlockModelManager.h
    Created: 10 Apr 2018 7:48:55pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MotionBlockModel.h"

class UserMotionBlockModelManager :
	public BaseManager<MotionBlockModel>
{
public:
	enum BlockType { PICTURE, NODE, SCRIPT, TIMELINE };

	UserMotionBlockModelManager(const String &name, BlockType type);
	~UserMotionBlockModelManager();

	BlockType type;
	File libFolder;

	MotionBlockModel * createItem();


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
};