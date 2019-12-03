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

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData) override;

	String getTypeString() const override { return "Position" ; }
	static PositionPattern * create(var params) { return new PositionPattern(params); }

};

class LinePattern :
	public PatternBlock
{
public:
	LinePattern(var params = var());
	~LinePattern() {}

	Point3DParameter * startPoint;
	Point3DParameter * endPoint;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData) override;

	String getTypeString() const override { return "Line"; } 
	static LinePattern * create(var params) { return new LinePattern(params); }
};

class CirclePattern :
	public PatternBlock
{
public:
	CirclePattern(var params = var());
	~CirclePattern() {}

	Point3DParameter * center;
	IntParameter* numCircles;
	FloatParameter* radius1;
	FloatParameter* radius2;

	Point3DParameter * orientation;

	FloatParameter* startAngle1;
	FloatParameter * startAngle2;
	FloatParameter* length1;
	FloatParameter * length2;

	FloatParameter* speed1;
	FloatParameter * speed2;

	Point3DParameter * offset;
	FloatParameter * radiusOffset;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData) override;

	String getTypeString() const override { return "Circle"; }
	static CirclePattern * create(var params) { return new CirclePattern(params); }
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

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData) override;

	String getTypeString() const override { return "Ping Pong"; }
	static PingPongPattern * create(var params) { return new PingPongPattern(params); }
};

class MultiPositionPattern :
	public PatternBlock
{
public:
	MultiPositionPattern(var params = var());
	~MultiPositionPattern() {}

	Point3DParameter * center;
	Point3DParameter * orientation;
	Point3DParameter * pos1;
	Point3DParameter * pos2;
	Point3DParameter * pos3;

	void getMotionDataInternal(var result, Drone * d, double time, int id, var params, var * blockMemoryData) override;

	static void createPosition(ControllableContainer *);

	String getTypeString() const override { return "Multi Position"; }
	static MultiPositionPattern * create(var params) { return new MultiPositionPattern(params); }
};