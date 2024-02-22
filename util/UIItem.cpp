#include "UIItem.h"
#include "FontsManager.h"
#include <cassert>

//コンストラクタ
UIItem::UIItem()
{
}

//デストラクタ
UIItem::~UIItem()
{
}

//サイズ調整、アルファ値の変更があるのUI描画
void UIItem::AlphaChangeDraw(const float scale, const int alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(centerPosX_, centerPosY_, scale, 0.0f, makeScreenHandle_, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//ポジションが変わるUIの描画
void UIItem::ChangePosDraw(const float centerPosX, const  float centerPosY)
{
	DrawRotaGraphF(centerPosX, centerPosY, 1.0f, 0.0f, makeScreenHandle_, true);
}

//画像を3D空間に描画する
void UIItem::DrawBillboard(const VECTOR& drawPos, const int alpha, const  float size)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBillboard3D(drawPos, 0.5f, 0.5f, size, 0.0f, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//UI画像を作成するための位置情報等を設定する
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

//UI画像の作成
void UIItem::CreateUIGraph(const int fontHandle)
{
	assert(fontHandle != -1);

	//短縮化
	auto& fontManager = FontsManager::GetInstance();

	//描画スクリーンの変更
	SetDrawScreen(makeScreenHandle_);

	//文字列の横の長さを取得
	int strWidth = fontManager.GetStringSize(UIString_.c_str(), fontManager.GetFontName(fontHandle).c_str());

	//フォントを適用した文字の描画
	DrawStringToHandle(makeScreenWidth_ / 2 - strWidth / 2, makeScreenHeight_ / 2, UIString_.c_str(), 0xffffff, fontHandle);

	SetDrawScreen(DX_SCREEN_BACK);
}
