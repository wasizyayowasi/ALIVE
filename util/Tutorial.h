#pragma once
#include <DxLib.h>


class Tutorial
{
public:

	Tutorial();
	virtual ~Tutorial();

	void Update(VECTOR pos);
	void Draw(bool inputDevice);

	void NoneDraw(bool inputDevice);
	void SwitchTutorialDraw(bool inputDevice);
	void CranckTutorialDraw(bool inputDevice);

private:

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)(bool inputDevice);

};

