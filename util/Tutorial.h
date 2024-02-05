#pragma once
#include "KeyData.h"
#include <DxLib.h>
#include <map>

class Tutorial
{
private:
	//UI画像の列挙
	enum class UIGraph {
		XboxBotton,
		KeyBord,
		max,
	};
	//チュートリアル
	enum class TutorialGimmick {
		Switch,
		Lever,
		Crank,
		max,
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tutorial();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Tutorial();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">プレイヤーのポジション</param>
	void Update(VECTOR playerPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// キーの画像を描画する
	/// </summary>
	/// <param name="num">キーの番号</param>
	void KeyGraphDraw(int keyNum);

	/// <summary>
	/// 何も描画しない
	/// </summary>
	void NoneDraw();

	/// <summary>
	/// スイッチのチュートリアル
	/// </summary>
	void SwitchTutorialDraw();

	/// <summary>
	/// クランクのチュートリアル
	/// </summary>
	void CranckTutorialDraw();

	/// <summary>
	/// 走る動作のチュートリアル
	/// </summary>
	void RunTutorialDraw();

	/// <summary>
	/// ジャンプのチュートリアル
	/// </summary>
	void JumpTutorialDraw();

	/// <summary>
	/// エレベーターのチュートリアル
	/// </summary>
	void ElevatorTutorialDraw();

private:
	
	int fotnPigumo42_ = 0;

	static constexpr int fadeInterval_ = 5;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::map<UIGraph,int> UIHandle_;					//画像
	std::map<UIGraph, std::pair<float, float>> UIPos_;	//UIの画面ポジション pairの中身｜first：X｜second：Y

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)();

};

