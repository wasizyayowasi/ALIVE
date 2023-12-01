#pragma once
#include <DxLib.h>


class Tutorial
{
public:

	Tutorial();
	virtual ~Tutorial();

	void Update(VECTOR pos);
	void Draw();

	void NoneDraw();
	void SwitchTutorialDraw();
	void CranckTutorialDraw();

private:

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)();

};

