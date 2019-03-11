/*
  ==============================================================================

    UserMotionBlockModelManager.cpp
    Created: 10 Apr 2018 7:48:55pm
    Author:  Ben

  ==============================================================================
*/

#include "UserMotionBlockModelManager.h"
#include "blocks/script/ScriptBlock.h"
#include "blocks/timeline/TimelineBlock.h"

#include "Timeline/TimelineEditor.h"

UserMotionBlockModelManager::UserMotionBlockModelManager(const String &name, BlockType type) :
	BaseManager(name),
	type(type)
{
	libFolder = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Bento/" + name);
	libFolder.createDirectory();
}

UserMotionBlockModelManager::~UserMotionBlockModelManager()
{
	
}

MotionBlockModel * UserMotionBlockModelManager::createItem()
{
	switch (type)
	{
	
	case SCRIPT: return new ScriptBlock();
	case TIMELINE:
	{
		TimelineBlock * t = new TimelineBlock();

		if (!Engine::mainEngine->isLoadingFile)
		{
			ShapeShifterManager::getInstance()->showContent(TimelineEditor::getTypeStringStatic());
			TimelineEditor * te = (TimelineEditor *)ShapeShifterManager::getInstance()->getContentForName(TimelineEditor::getTypeStringStatic());
			if (te != nullptr) te->setSequence(&t->sequence);
		}
		
		return t;
	}

	default:
		break;
	}
	jassertfalse;
	return nullptr;
}

var UserMotionBlockModelManager::getJSONData()
{
	var data = BaseManager::getJSONData();

	/*
	MotionBlockModel * editingBlock = nullptr;
	for (auto & i : items)
	{
		if (i->isBeingEdited)
		{
			editingBlock = i;
			break;
		}
	}
	*/

	//if (editingBlock != nullptr) data.getDynamicObject()->setProperty("editingBlock", editingBlock->shortName);
	return data;
}

void UserMotionBlockModelManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	/*if (data.hasProperty("editingBlock"))
	{
		
		MotionBlockModel * m = getItemWithName(data.getProperty("editingBlock", ""));

		if (NodeBlock * nb = dynamic_cast<NodeBlock *>(m))
		{
			DBG("Edit node block");
			NodeBlockEditor * ne = (NodeBlockEditor *)ShapeShifterManager::getInstance()->getContentForName(NodeBlockEditor::getTypeStringStatic());
			if (ne != nullptr) ne->setNodeBlock(nb);
		}else if(TimelineBlock * t = dynamic_cast<TimelineBlock *>(m))
		{
			DBG("Edit timeline block"); 
			TimelineEditor * te = (TimelineEditor *)ShapeShifterManager::getInstance()->getContentForName(TimelineEditor::getTypeStringStatic());
			if (te != nullptr) te->setSequence(&t->sequence);
		}
		
	}*/
}
