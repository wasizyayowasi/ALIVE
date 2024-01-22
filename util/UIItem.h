#pragma once
#include <DxLib.h>

class UIItem
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIItem();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UIItem();

	/// <summary>
	/// サイズ調整、アルファ値の変更があるのUI描画
	/// </summary>
	/// <param name="scale">サイズ</param>
	/// <param name="alpha">アルファ値</param>
	void AlphaChangeDraw(float scale, int alpha);

	/// <summary>
	/// UI画像を作成するための位置情報等を設定する
	/// </summary>
	/// <param name="centerPosX">ポジションのX</param>
	/// <param name="centerPosY">ポジションのY</param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="str">文字列</param>
	/// <param name="fontHandle">フォントハンドル</param>
	void CreateUIGraphSetUp(float centerPosX, float centerPosY, int width, int height, const char* str, int fontHandle);

	/// <summary>
	/// UI画像の作成
	/// </summary>
	/// <param name="fontHandle">フォントハンドル</param>
	void CreateUIGraph(int fontHandle);

private:
	
	int makeScreenHandle_ = 0;
	int makeScreenWidth_ = 0;
	int makeScreenHeight_ = 0;

	float centerPosX_ = 0;
	float centerPosY_ = 0;
	float scale_ = 1.0f;

	const char* UIString = nullptr;

};

