#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class ManualCrank
{
public:
	ManualCrank();
	virtual ~ManualCrank();

	void Update();
	void Draw();

private:

	float rotationZ_ = 0.0f;

	VECTOR pos_ = { -200, 100, 0 };
	VECTOR rot_ = {};

	std::shared_ptr<Model> model_;

};

