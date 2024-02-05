#pragma once
#include <DxLib.h>

class Effect2D
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	/// <param name="numX">横の分割数</param>
	/// <param name="numY">縦の分割数</param>
	/// <param name="graphSizeX">1つの画像の横のサイズ</param>
	/// <param name="graphSizeY">1つの画像の縦のサイズ</param>
	/// <param name="pos">描画場所</param>
	Effect2D(const char* const filepath, int numX, int numY, int graphSizeX, int graphSizeY, float size, VECTOR pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Effect2D();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 画像を3D空間に描画する
	/// </summary>
	void DrawBillboard();

	/// <summary>
	/// 存在しているかどうか取得する
	/// </summary>
	/// <returns>true：している　false：していない</returns>
	bool IsEnable() { return isEnabled_; }
private:

	int elapsedTime_ = 0;			//経過時間
	int currentNum_ = 0;			//現在の番号
	int totalNum_ = 0;				//分割数の数

	int handle_[99] = {};			//画像用の配列

	float drawSize_ = 0.0f;			//描画サイズ

	bool isEnabled_ = false;		//存在しているか

	VECTOR drawPos_ = {};			//描画場所
};

