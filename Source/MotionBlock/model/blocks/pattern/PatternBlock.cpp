/*
  ==============================================================================

	PatternBlock.cpp
	Created: 10 Apr 2018 6:58:57pm
	Author:  Ben

  ==============================================================================
*/

#include "PatternBlock.h"
#include "MotionBlock/MotionBlock.h"
#include "Drone/DroneManager.h"

PatternBlock::PatternBlock(const String &name, var params) :
	MotionBlockModel(name == "" ? getTypeString() : name, params, PATTERN)
{
	saveAndLoadName = false;
}

PatternBlock::~PatternBlock()
{
}

PositionPattern::PositionPattern(var params) :
	PatternBlock(getTypeString(), params)
{
	position = paramsContainer->addPoint3DParameter("Position", "The target position");
	position->setVector(0, 2, 0);
	positionIDOffset = paramsContainer->addPoint3DParameter("Offset By ID", "The offset multiplied by the drone id");
	positionIDOffset->setValue(.5f, 0, 0);
}

void PositionPattern::getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData)
{
	Vector3D<float> bPosition = getPoint3DValue(position, params);
	Vector3D<float> bPositionIDOffset = getPoint3DValue(positionIDOffset, params);
	Vector3D<float> tPos = bPosition + bPositionIDOffset * id;
	var posData = getVarForPoint3D(tPos);
	result.getDynamicObject()->setProperty("position", posData);
}


LinePattern::LinePattern(var params) :
	PatternBlock(getTypeString(), params)
{
	startPoint = paramsContainer->addPoint3DParameter("Start position", "Position of drone ID 0");
	startPoint->setVector(-3, 2, 0);
	endPoint = paramsContainer->addPoint3DParameter("End Position", "Position of last drone in group");
	endPoint->setValue(3, 2, 0);
}

void LinePattern::getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData)
{
	Vector3D<float> start = getPoint3DValue(startPoint, params);
	Vector3D<float> end = getPoint3DValue(endPoint, params);
	float relP = id * 1.0f / jmax((getDroneCount(params) - 1), 1);

	Vector3D<float> tPos = start + (end - start)*relP;
	var posData = getVarForPoint3D(tPos);
	result.getDynamicObject()->setProperty("position", posData);
}

CirclePattern::CirclePattern(var params) :
	PatternBlock(getTypeString(), params)
{
	center = paramsContainer->addPoint3DParameter("Center", "The center of the circle");
	center->setVector(0, .5f, 0);

	numCircles = paramsContainer->addIntParameter("Num Circles", "Number of circles to do", 1, 1, 10);

	radius1 = paramsContainer->addFloatParameter("Radius 1", "Radius of the big circle", 0, 0, 2);
	startAngle1 = paramsContainer->addFloatParameter("Start Angle 1", "Start angle, 1 is full circle, .5 is half", 0, 0, 1);
	speed1 = paramsContainer->addFloatParameter("Speed 1", "Speed of the motion, in full circles per second (1 is 1 full round per second)", 0, -2, 2);
	length1 = paramsContainer->addFloatParameter("Length 1", "Length to cover, 1 is full circle, .5 is arc of half circle", 1, 0, 1);

	radius2 = paramsContainer->addFloatParameter("Radius 2", "Radius of the small circles", .3f, 0 ,10);
	startAngle2 = paramsContainer->addFloatParameter("Start Angle 2", "Start angle, 1 is full circle, .5 is half", 0, 0, 1);
	speed2 = paramsContainer->addFloatParameter("Speed 2", "Speed of the motion, in full circles per second (1 is 1 full round per second)", .1, -2, 2);
	length2 = paramsContainer->addFloatParameter("Length 2", "Length to cover, 1 is full circle, .5 is arc of half circle", 1, 0, 1);

	offset = paramsContainer->addPoint3DParameter("Offset", "Offset per ID");
	radiusOffset = paramsContainer->addFloatParameter("Radius Offset", "Radius offset by ID", 0,-.5f,.5f);
	orientation = paramsContainer->addPoint3DParameter("Orientation", "Orientation of the circle, this is the Up vector, circle is perpendicular to this vector");
	orientation->setVector(0, 1, 0);
}

void CirclePattern::getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData)
{
	Vector3D<float> bCenter = getPoint3DValue(center, params);
	//Vector3D<float> bOrientation = getPoint3DValue(orientation, params);

	float bNumCircles = getParamValue<int>(numCircles, params);
	float bRadius1 = getParamValue<float>(radius1, params);
	float bSpeed1 = getParamValue<float>(speed1, params);
	float bStartAngle1 = getParamValue<float>(startAngle1, params);
	float bLength1 = getParamValue<float>(length1, params);

	float bRadius2 = getParamValue<float>(radius2, params);
	float bSpeed2 = getParamValue<float>(speed2, params);
	float bStartAngle2 = getParamValue<float>(startAngle2, params);
	float bLength2 = getParamValue<float>(length2, params);

	int numDrones = getDroneCount(params);
	int numDronesPerCircle = jmax<int>(ceil(numDrones / bNumCircles), 1);
	int curBigCircle = floor(id / numDronesPerCircle);

	
	float relP1 = curBigCircle * 1.0f / bNumCircles * bLength1;
	float relAngle1 = bStartAngle1 + relP1 + time * bSpeed1;
	float tAngle1 = fmodf(relAngle1, 1) * float_Pi * 2;
	Vector3D<float> tPos = bCenter + Vector3D<float>(cosf(tAngle1), 0, sinf(tAngle1)) * bRadius1;



	float relP2 = (id%numDronesPerCircle) * 1.0f / numDronesPerCircle * bLength2;
	float relAngle2 = bStartAngle2 + relP2 + time * bSpeed2;
	float tAngle2 = fmodf(relAngle2, 1)  * float_Pi * 2 ;

	Vector3D<float> bOffset = getPoint3DValue(offset, params);
	float bRadiusOffset = getParamValue<float>(radiusOffset, params);

	tPos += Vector3D<float>(cosf(tAngle2), 0, sinf(tAngle2)) * (bRadius2 + bRadiusOffset * id) + bOffset * id;

	var posData;
	posData.append(tPos.x);
	posData.append(tPos.y);
	posData.append(tPos.z);
	result.getDynamicObject()->setProperty("position", posData);
}

PingPongPattern::PingPongPattern(var params) :
	PatternBlock(getTypeString(), params)
{
	startPosition = paramsContainer->addPoint3DParameter("Start Position", "The start position");
	startPosition->setVector(0, 2, -.5f);

	endPosition = paramsContainer->addPoint3DParameter("End Position", "The end position");
	endPosition->setVector(0, 2, .5f);

	speed = paramsContainer->addFloatParameter("Speed", "Speed of the ping pong animation, in cycles per second (1 = 1 round trip per sec)", 1);
	
	positionIDOffset = paramsContainer->addPoint3DParameter("Offset By ID", "The offset multiplied by the drone id");
	positionIDOffset->setValue(.5f, 0, 0);

	timeIDOffset = paramsContainer->addFloatParameter("Offset Time By ID", "Offset the time by the drone id", 0);
}

void PingPongPattern::getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData)
{
	Vector3D<float> start = getPoint3DValue(startPosition, params);
	Vector3D<float> end = getPoint3DValue(endPosition, params);
	Vector3D<float> bPositionIDOffset = getPoint3DValue(positionIDOffset, params);
	float bSpeed = getParamValue<float>(speed, params);
	float bTimeIDOffset = getParamValue<float>(timeIDOffset, params);

	float relPos = sinf((time * bSpeed + bTimeIDOffset*id)* float_Pi * 2)*.5f+.5f;

	Vector3D<float> tPos = start + (end - start)*relPos + bPositionIDOffset * id;

	var posData = getVarForPoint3D(tPos);
	result.getDynamicObject()->setProperty("position", posData);
}

MultiPositionPattern::MultiPositionPattern(var params) :
	PatternBlock(getTypeString(), params)
{
	center = paramsContainer->addPoint3DParameter("Center", "Center of the shape");
	orientation = paramsContainer->addPoint3DParameter("Orientation", "Orientation of the shape");
	pos1 = paramsContainer->addPoint3DParameter("Position 1", "Position of first drone");
	pos2 = paramsContainer->addPoint3DParameter("Position 2", "Position of second drone");
	pos3 = paramsContainer->addPoint3DParameter("Position 3", "Position of fourth drone drone. Kidding, third.");
	//paramsContainer->userCanAddControllables = true;
	//paramsContainer->customUserCreateControllableFunc = &MultiPositionPattern::createPosition;
	//paramsContainer->userAddControllablesFilters.add(Point3DParameter::getTypeStringStatic());
}

void MultiPositionPattern::getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData)
{
	Vector3D<float> bCenter = getPoint3DValue(center, params);
	//Vector3D<float> bOrientation = getPoint3DValue(orientation, params);

	var pos = params.getProperty("position" + String(id+1), var());
	if (!pos.isArray()) return;
	
	Vector3D<float> bPos(pos[0], pos[1], pos[2]);
	Vector3D<float> targetPos = bCenter + bPos;
	var rPos;
	rPos.append(targetPos.x);
	rPos.append(targetPos.y);
	rPos.append(targetPos.z);
	result.getDynamicObject()->setProperty("position", rPos);
}

void MultiPositionPattern::createPosition(ControllableContainer * cc)
{
	Point3DParameter * p = cc->addPoint3DParameter("Position "+String(cc->controllables.size()), "Position");
	p->isRemovableByUser = true;
	p->saveValueOnly = false;
	cc->orderControllablesAlphabetically();
	int id = 0;
	for (auto &c : cc->controllables)
	{
		if (!c->shortName.startsWith("position")) continue;
		c->setNiceName("Position " + String(id));
		id++;
	}
}
