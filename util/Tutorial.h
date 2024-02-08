#pragma once
#include <DxLib.h>
#include <map>

class InputState;

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
	/// 何も描画しない
	/// </summary>
	/// <param name="input">InputStateクラスの参照</param>
	void NoneDraw(InputState& input);

	/// <summary>
	/// スイッチのチュートリアル
	/// </summary>
	/// <param name="input">InputStateクラスの参照</param>
	void SwitchTutorialDraw(InputState& input);

	/// <summary>
	/// クランクのチュートリアル
	/// </summary>
	/// <param name="input">InputStateクラスの参照</param>
	void CranckTutorialDraw(InputState& input);

	/// <summary>
	/// 走る動作のチュートリアル
	/// </summary>
	/// <param name="input">InputStateクラスの参照</param>
	void RunTutorialDraw(InputState& input);

	/// <summary>
	/// ジャンプのチュートリアル
	/// </summary>
	/// <param name="input">InputStateクラスの参照</param>
	void JumpTutorialDraw(InputState& input);

	/// <summary>
	/// エレベーターのチュートリアル
	/// </summary>
	/// <param name="input">InputStateクラスの参照</param>
	void ElevatorTutorialDraw(InputState& input);

	/// <summary>
	/// 死体の足場のチュートリアル
	/// </summary>
	/// <param name="input">InputStateクラスの参照</param>
	void CorpseScaffoldDraw(InputState& input);
private:
	
	int fontPigumo42_ = 0;

	static constexpr int fadeInterval_ = 5;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::map<UIGraph,int> UIHandle_;					//画像
	std::map<UIGraph, std::pair<float, float>> UIPos_;	//UIの画面ポジション pairの中身｜first：X｜second：Y

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)(InputState& input);

};

