#pragma once
#include "../util/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

enum class AnimType {
	on,
	off,
	max,
};

class Lever
{
public:

	Lever(LoadObjectInfo info,VECTOR stopPos);
	virtual ~Lever();

	void Update();
	void Draw();

	bool CollCheck(VECTOR playerPos);

	VECTOR GetElevatorStopPoint() { return elevatorStopPosition_; }

	std::shared_ptr<Model> GetModelPointer() { return model_; }

	void OnAnimation();

	void OffAnimation();

	bool IsOn() { return isOn_; }

private:

	bool isOn_ = false;

	VECTOR pos_ = {};
	VECTOR elevatorStopPosition_ = {};

	std::shared_ptr<Model> model_;

};

