#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class InputState;
class Player;

enum class State {
	top,
	left,
	bottom,
	right,
};

class ManualCrank
{
public:
	ManualCrank();
	virtual ~ManualCrank();

	void Update(const InputState& input);
	void Draw();

	//回転の更新
	void AngleUpdate(const InputState& input);

	//※現在使えてない※
	//現在の状態を角度によって変化させる
	void CurrentStateUpdate();

	//次に移動できる角度の状態を更新する
	void NextStateUpdate();

	//プレイヤーとモデルの当たり判定を行う
	bool HitCollPlayer(Player& player);

	std::shared_ptr<Model>GetModelPointer() { return model_; }

	//現在のZ軸の回転率を取得する
	float GetRotZ() { return rotZ_; }

	//Z軸の最大回転率を取得する
	float GetMaxRotZ();

private:

	int crankLaps_ = 0;

	float rotZ_ = 0.0f;

	VECTOR pos_ = { -200, 100, 0 };
	VECTOR rot_ = {};

	State currentState_ = {};

	State turnRight_ = {};
	State turnLeft_ = {};

	std::shared_ptr<Model> model_;

};

