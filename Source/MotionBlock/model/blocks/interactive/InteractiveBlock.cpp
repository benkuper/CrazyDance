/*
  ==============================================================================

    InteractiveBlock.cpp
    Created: 28 Nov 2019 11:44:52am
    Author:  bkupe

  ==============================================================================
*/

#include "InteractiveBlock.h"
#include "Drone/Drone.h"

InteractiveBlock::InteractiveBlock(var params) :
	MotionBlockModel(getTypeString(),params,MotionBlockModel::Type::INTERACTIVE)
{
	useTargetPosition = paramsContainer->addBoolParameter("Use Target Position","Use target instead of real position",false);
}

InteractiveBlock::~InteractiveBlock()
{
}

void InteractiveBlock::getMotionDataInternal(var result, Drone* d, double time, int id, var params, var * blockMemoryData)
{
	bool useTarget = getParamValue<bool>(useTargetPosition, params);

	bool isLocked = params.getProperty("isTransition", false);
	
	String lockedPosParamName = "lockedPos" + d->globalID->stringValue();
	var lockedPos;
	if (isLocked)
	{
		if (!blockMemoryData->hasProperty(lockedPosParamName))
		{
			for (int i = 0; i < 3; i++) lockedPos.append(0);
		}
		else lockedPos = blockMemoryData->getDynamicObject()->getProperty(lockedPosParamName);
	}

	Vector3D<float> tPos = isLocked ? Vector3D<float>(lockedPos[0], lockedPos[1], lockedPos[2]) : (useTarget ? d->targetPosition : d->realPosition)->getVector();
	
	if (!isLocked)
	{
		lockedPos.append(tPos.x);
		lockedPos.append(tPos.y);
		lockedPos.append(tPos.z);
		blockMemoryData->getDynamicObject()->setProperty(lockedPosParamName, lockedPos);
	}
	
	var posData = getVarForPoint3D(tPos);
	result.getDynamicObject()->setProperty("position", posData);
}
