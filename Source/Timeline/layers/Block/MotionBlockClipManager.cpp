/*
  ==============================================================================

	MotionBlockClipManager.cpp
	Created: 17 Apr 2018 5:11:44pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockClipManager.h"
#include "MotionBlockLayer.h"

MotionBlockClipManager::MotionBlockClipManager(MotionBlockLayer * layer) :
	LayerBlockManager(layer, "Clips"),
	layer(layer)
{
	itemDataType = "MotionBlockClip";
	blocksCanOverlap = true;
}


MotionBlockClipManager::~MotionBlockClipManager()
{
}

LayerBlock * MotionBlockClipManager::createItem()
{
	return new MotionBlockClip(layer);
}

void MotionBlockClipManager::getSurroundingBlocks(MotionBlockClip * clip, MotionBlockClip *& prevClip, MotionBlockClip *& nextClip)
{
	int index = items.indexOf(clip);
	if (index == -1) return;
	if (index > 0) prevClip = (MotionBlockClip *)(items[index - 1]);
	if (index < items.size() - 1) nextClip = (MotionBlockClip *)(items[index + 1]);
}

void MotionBlockClipManager::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	MotionBlockClip* b = c->getParentAs<MotionBlockClip>();
	if (b != nullptr)
	{
		if (c == b->time || c == b->coreLength || c == b->loopLength)
		{
			//if (!blocksCanOverlap) return;
			//computeFadesForClip(b, true);
		}
	}
}
/*
void MotionBlockClipManager::computeFadesForClip(MotionBlockClip * clip, bool propagate)
{
	int bIndex = items.indexOf(clip);

	MotionBlockClip * prevBlock = bIndex > 0 ? (MotionBlockClip *)items[bIndex - 1] : nullptr;
	MotionBlockClip * nextBlock = bIndex < items.size() - 1 ? (MotionBlockClip *)items[bIndex + 1] : nullptr;

	if (prevBlock != nullptr && prevBlock->time->floatValue() > clip->time->floatValue())
	{
		reorderItems();
		computeFadesForClip(clip, propagate);
		return;
	}

	if (nextBlock != nullptr && nextBlock->time->floatValue() < clip->time->floatValue())
	{
		reorderItems();
		computeFadesForClip(clip, propagate);
		return;
	}

	if (clip->autoFade->boolValue())
	{
		float fadeIn = prevBlock == nullptr ? 0 : jmax(prevBlock->getEndTime() - clip->time->floatValue(), 0.f);
		float fadeOut = nextBlock == nullptr ? 0 : jmax(clip->getEndTime() - nextBlock->time->floatValue(), 0.f);

		clip->fadeIn->setValue(fadeIn);
		clip->fadeOut->setValue(fadeOut);
	}

	if (propagate)
	{
		if (prevBlock != nullptr) computeFadesForClip(prevBlock, false);
		if (nextBlock != nullptr) computeFadesForClip(nextBlock, false);
	}
}
*/