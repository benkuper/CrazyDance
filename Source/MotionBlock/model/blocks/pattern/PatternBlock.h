/*
  ==============================================================================

    PatternBlock.h
    Created: 10 Apr 2018 6:58:57pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MotionBlockModel.h"
#include "PerlinNoise.hpp"

class PatternBlock :
	public MotionBlockModel
{
public:
	PatternBlock(const String &name = "Pattern", var params = var());
	~PatternBlock();

	String getTypeString() const override { return "Pattern"; }
};


//Patterns

class PositionPattern :
	public PatternBlock
{
public:
	PositionPattern(var params = var());
	~PositionPattern() {}

	Point3DParameter * position;
	Point3DParameter * positionIDOffset;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params) override;

	String getTypeString() const override { return "Position" ; }
};

class LinePattern :
	public PatternBlock
{
public:
	LinePattern(var params = var());
	~LinePattern() {}

	Point3DParameter * startPoint;
	Point3DParameter * endPoint;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params) override;

	String getTypeString() const override { return "Line"; } 
};

class CirclePattern :
	public PatternBlock
{
public:
	CirclePattern(var params = var());
	~CirclePattern() {}

	Point3DParameter * center;
	FloatParameter * radius;
	Point3DParameter * orientation;
	FloatParameter * startAngle;
	FloatParameter * length;
	FloatParameter * speed;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params) override;

	String getTypeString() const override { return "Circle"; }
};


class PingPongPattern :
	public PatternBlock
{
public:
	PingPongPattern(var params = var());
	~PingPongPattern() {}

	EnumParameter * mode;
	Point3DParameter * startPosition;
	Point3DParameter * endPosition;
	FloatParameter * speed;
	Point3DParameter * positionIDOffset;
	FloatParameter * timeIDOffset;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params) override;

	String getTypeString() const override { return "Ping Pong"; }
};

class MultiPositionPattern :
	public PatternBlock
{
public:
	MultiPositionPattern(var params = var());
	~MultiPositionPattern() {}

	Point3DParameter * center;
	Point3DParameter * orientation;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params) override;

	static void createPosition(ControllableContainer *);

	String getTypeString() const override { return "Multi Position"; }
};