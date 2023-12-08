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

	void UndoRotation(float rotZ);

	std::shared_ptr<Model> GetModelPointer() { return model_; }

	float GetRotZ() { return rotZ_; }

	void SetRotZ(float rotZ) { rotZ_ = rotZ; }

	float GetMaxRotZ();

	void SetOperate(bool isOperate);

private:

	float rotZ_ = -40.0f;

	bool isOperated_ = false;

	VECTOR pos_ = {};
	VECTOR elevatorStopPosition_ = {};

	std::shared_ptr<Model> model_;

};

