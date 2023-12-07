#pragma once
#include "../util/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

class Lever
{
public:

	Lever(LoadObjectInfo info,VECTOR stopPos);
	virtual ~Lever();

	void Update();
	void Draw();

	bool CollCheck(VECTOR playerPos);

	VECTOR GetElevatorStopPoint() { return elevatorStopPosition_; }

private:

	VECTOR pos_ = {};
	VECTOR elevatorStopPosition_ = {};

	std::shared_ptr<Model> model_;

};

