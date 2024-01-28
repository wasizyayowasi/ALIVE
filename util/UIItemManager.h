#pragma once
#include <DxLib.h>
#include <map>
#include <vector>
#include <memory>
#include <string>

class UIItem;

class UIItemManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIItemManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UIItemManager();

	/// <summary>
	/// 引数で送られてきた番号だけ
	/// 拡縮率を変更するときの描画
	/// </summary>
	/// <param name="selectNum">番号</param>
	void AlphaChangeDraw(int selectNum,int fadeAlphaValue);

	/// <summary>
	/// ポジションがを変更するUIの描画
	/// </summary>
	/// <param name="centerPosX">ポジションX</param>
	/// <param name="centerPosY">ポジションY</param>
	void ChangePosDraw(float centerPosX, float centerPosY);

	/// <summary>
	/// 画像を3D空間に描画する
	/// </summary>
	/// <param name="drawPos">描画座標</param>
	/// <param name="alpha">アルファ値</param>
	/// /// <param name="size">サイズ</param>
	void DrawBillBoard(std::map<std::string,VECTOR> drawPos, float alpha,float size);

	/// <summary>
	/// 画像を3D空間に描画する
	/// </summary>
	/// <param name="drawPos">描画座標</param>
	/// <param name="selectNum">選択番号</param>
	/// <param name="fadeValue">アルファ値</param>
	/// /// <param name="size">サイズ</param>
	void AlphaChangeDrawBillBoard(std::map<std::string, VECTOR> drawPos, int selectNum, int fadeValue, float size);

	/// <summary>
	/// UIメニューの追加
	/// </summary>
	/// <param name="centerPosX">ポジションのX</param>
	/// <param name="centerPosY">ポジションのY</param>
	/// <param name="width">スクリーンの横のサイズ</param>
	/// <param name="height">スクリーンの縦のサイズ</param>
	/// <param name="str">文字列</param>
	/// <param name="fontHandle">フォントハンドル</param>
	void AddMenu(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle);

	/// <summary>
	/// 文字列を分割したメニューの追加
	/// </summary>
	/// <param name="centerPosX">ポジションのX</param>
	/// <param name="centerPosY">ポジションのY</param>
	/// <param name="width">スクリーンの横のサイズ</param>
	/// <param name="height">スクリーンの縦のサイズ</param>
	/// <param name="str">文字列</param>
	/// <param name="fontHandle">フォントハンドル</param>
	void AddingMenuWithSplitStr(float centerPosX, float centerPosY, int width, int height, std::string str, int fontHandle);

private:

	std::vector<std::shared_ptr<UIItem>> UIMenu_;			//UIItemのスマートポインタのvector配列
	std::map<std::string, int>	fontSizeWithAppliedFont_;	//フォントを適用した文字の大きさ

};

