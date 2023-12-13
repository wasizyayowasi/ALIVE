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
	//コンストラクタ
	ManualCrank(LoadObjectInfo objInfo);
	//デストラクタ
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

	//クランクを回す際の立ち位置の取得
	VECTOR GetStandingPosition() { return standingPos_; }

private:

	float rotZ_ = 0.0f;					//Z軸の回転量
	float radian_ = 0.0f;				//ラジアン

	VECTOR pos_ = {};					//ポジション
	VECTOR initPos_ = {};				//初期ポジション
	VECTOR standingPos_ = {};			//クランクを回す立ち位置

	std::shared_ptr<Model> model_;		//モデルクラスのスマートポインタ

};

