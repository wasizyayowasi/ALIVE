#pragma once
#include "DxLib.h"

class UIItem
{
public:

	UIItem();
	virtual ~UIItem();

	void draw(float scale, int alpha);

	void createUIGraphSetUp(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle);

	void createUIGraph(int fontHandle);

private:
	
	int makeScreenHandle_ = 0;
	int makeScreenWidth_ = 0;
	int makeScreenHeight_ = 0;

	float centerPosX_ = 0;
	float centerPosY_ = 0;
	float scale_ = 1.0f;

	const char* UIString = nullptr;

};

