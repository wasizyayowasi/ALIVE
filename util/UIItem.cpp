#include "UIItem.h"
#include "../util/FontsManager.h"
#include <cassert>

UIItem::UIItem()
{
}

UIItem::~UIItem()
{
}

void UIItem::draw(float scale,int alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(centerPosX_, centerPosY_, scale, 0.0f, makeScreenHandle_, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UIItem::createUIGraphSetUp(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle)
{
	makeScreenWidth_ = width;
	makeScreenHeight_ = height;
	centerPosX_ = centerPosX;
	centerPosY_ = centerPosY;

	UIString = str;

	makeScreenHandle_ = MakeScreen(makeScreenWidth_, makeScreenHeight_, true);
	createUIGraph(fontHandle);

}

void UIItem::createUIGraph(int fontHandle)
{
	assert(fontHandle != -1);

	auto& fontManager = FontsManager::getInstance();

	SetDrawScreen(makeScreenHandle_);

	int strWidth = fontManager.GetStringSize(UIString, fontManager.GetFontName(fontHandle).c_str());
	DrawStringToHandle(makeScreenWidth_ / 2 - strWidth / 2, makeScreenHeight_ / 2, UIString, 0xffffff, fontHandle);

	SetDrawScreen(DX_SCREEN_BACK);
}
