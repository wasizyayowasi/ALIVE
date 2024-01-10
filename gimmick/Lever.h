#pragma once
#include "../object/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

class Lever
{
private:

	enum class AnimType {
		on,
		off,
		max,
	};

public:
	//コンストラクタ
	Lever(LoadObjectInfo info,VECTOR stopPos);
	//デストラクタ
	virtual ~Lever();

	//更新
	void Update();
	//描画
	void Draw();

	//衝突判定
	bool CollCheck(VECTOR playerPos);

	//エレベータの停止位置の取得
	VECTOR GetElevatorStopPoint() { return elevatorStopPosition_; }

	//モデルポインターの取得
	std::shared_ptr<Model> GetModelPointer() { return model_; }
	
	//レバーを引くポジションの取得
	VECTOR GetStandingPosition() { return standingPos_; }

	//レバーの起動
	void OnAnimation();

	//レバーの停止
	void OffAnimation();

	//レバーが起動しているかどうか取得
	bool IsOn() { return isOn_; }
private:

	bool isOn_ = false;							//レバーが起動しているかどうか

	VECTOR pos_ = {};							//ポジション
	VECTOR elevatorStopPosition_ = {};			//エレベータの停止ポジション
	VECTOR standingPos_ = {};					//エレベータの停止ポジション

	std::shared_ptr<Model> model_;				//モデルクラスのスマートポインタ

};

