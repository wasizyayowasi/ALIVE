#include "UIItem.h"
#include "FontsManager.h"
#include <cassert>

//�R���X�g���N�^
UIItem::UIItem()
{
}

//�f�X�g���N�^
UIItem::~UIItem()
{
}

//�T�C�Y�����A�A���t�@�l�̕ύX�������UI�`��
void UIItem::AlphaChangeDraw(const float scale, const int alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(centerPosX_, centerPosY_, scale, 0.0f, makeScreenHandle_, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�|�W�V�������ς��UI�̕`��
void UIItem::ChangePosDraw(const float centerPosX, const  float centerPosY)
{
	DrawRotaGraphF(centerPosX, centerPosY, 1.0f, 0.0f, makeScreenHandle_, true);
}

//�摜��3D��Ԃɕ`�悷��
void UIItem::DrawBillboard(const VECTOR& drawPos, const int alpha, const  float size)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBillboard3D(drawPos, 0.5f, 0.5f, size, 0.0f, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//UI�摜���쐬���邽�߂̈ʒu��񓙂�ݒ肷��
void UIItem::CreateUIGraphSetUp(const float centerPosX, const float centerPosY, const int width, const int height, const std::string& str, const int fontHandle)
{
	makeScreenWidth_ = width;
	makeScreenHeight_ = height;
	centerPosX_ = centerPosX;
	centerPosY_ = centerPosY;

	UIString_ = str;

	makeScreenHandle_ = MakeScreen(makeScreenWidth_, makeScreenHeight_, true);
	CreateUIGraph(fontHandle);

}

//UI�摜�̍쐬
void UIItem::CreateUIGraph(const int fontHandle)
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
