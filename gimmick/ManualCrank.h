#pragma once
#include "../util/ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class InputState;
class Player;

class ManualCrank
{
public:
	ManualCrank(LoadObjectInfo objInfo);
	virtual ~ManualCrank();

	//描画
	void Draw();

	//プレイヤーとモデルの当たり判定を行う
	bool HitCollPlayer(Player& player);

	//モデルポインタを取得
	std::shared_ptr<Model>GetModelPointer() { return model_; }

	VECTOR GetInitPos() { return initPos_; }

	//現在のZ軸の回転率を取得する
	float GetRotZ() { return rotZ_; }

	//Z軸の最大回転率を取得する
	float GetMaxRotZ();

	//回転率を設定する
	void SetRotZ(float rotZ) { rotZ_ = rotZ; }

private:

	float rotZ_ = 0.0f;

	VECTOR pos_ = {};
	VECTOR initPos_ = {};

	std::shared_ptr<Model> model_;

};

