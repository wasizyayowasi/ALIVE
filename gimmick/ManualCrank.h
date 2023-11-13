#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class InputState;
class Player;

class ManualCrank
{
public:
	ManualCrank();
	virtual ~ManualCrank();

	//描画
	void Draw();

	//プレイヤーとモデルの当たり判定を行う
	bool HitCollPlayer(Player& player);

	//モデルポインタを取得
	std::shared_ptr<Model>GetModelPointer() { return model_; }

	//現在のZ軸の回転率を取得する
	float GetRotZ() { return rotZ_; }

	//Z軸の最大回転率を取得する
	float GetMaxRotZ();

	//回転率を設定する
	void SetRotZ(float rotZ) { rotZ_ = rotZ; }

private:

	float rotZ_ = 0.0f;

	VECTOR pos_ = { -200, 100, 0 };

	std::shared_ptr<Model> model_;

};

