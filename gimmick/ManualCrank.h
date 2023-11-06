#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class InputState;

enum class State {
	top,
	left,
	bottom,
	right,
};

class ManualCrank
{
public:
	ManualCrank();
	virtual ~ManualCrank();

	void Update(const InputState& input);
	void Draw();

	void AngleUpdate(const InputState& input);

	void CurrentStateUpdate();

private:

	float rotZ_ = 0.0f;

	VECTOR pos_ = { -200, 100, 0 };
	VECTOR rot_ = {};

	State currentState_ = {};

	std::shared_ptr<Model> model_;

};

