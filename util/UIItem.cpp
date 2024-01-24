#include "UIItem.h"
#include "FontsManager.h"
#include <cassert>

UIItem::UIItem()
{
}

UIItem::~UIItem()
{
}

void UIItem::AlphaChangeDraw(float scale,int alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(centerPosX_, centerPosY_, scale, 0.0f, makeScreenHandle_, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UIItem::ChangePosDraw(float centerPosX, float centerPosY)
{
	DrawRotaGraph(centerPosX, centerPosY, 1.0f, 0.0f, makeScreenHandle_, true);
}

void UIItem::CreateUIGraphSetUp(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle)
{
	makeScreenWidth_ = width;
	makeScreenHeight_ = height;
	centerPosX_ = centerPosX;
	centerPosY_ = centerPosY;

	UIString_ = str;

	makeScreenHandle_ = MakeScreen(makeScreenWidth_, makeScreenHeight_, true);
	CreateUIGraph(fontHandle);

}

void UIItem::CreateUIGraph(int fontHandle)
{
	assert(fontHandle != -1);

	//�Z�k��
	auto& fontManager = FontsManager::GetInstance();

	//�`��X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);

	//������̉��̒������擾
	int strWidth = fontManager.GetStringSize(UIString_.c_str(), fontManager.GetFontName(fontHandle).c_str());

	//�t�H���g��K�p���������̕`��
	DrawStringToHandle(makeScreenWidth_ / 2 - strWidth / 2, makeScreenHeight_ / 2, UIString_.c_str(), 0xffffff, fontHandle);

	SetDrawScreen(DX_SCREEN_BACK);
}
