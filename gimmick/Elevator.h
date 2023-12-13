#pragma once
#include "GimmickBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Switch;
class Lever;
class Model;

class Elevator : public GimmickBase
{
private:

	enum class ElevatorAnimType {
		openIdle,
		closeIdle,
		open,
		close,
		max,
	};

public:
	//コンストラクタ
	Elevator(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~Elevator();

	//更新
	void Update(Player& player, const InputState& input)override;
	//描画
	void Draw();

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

private:

	/// <summary>
	/// エレベーターを移動させる
	/// </summary>
	void Move();

	/// <summary>
	/// 移動先のポジションを取得する
	/// </summary>
	void TargetPosition();

private:

	float moveVecY_ = 0.0f;							//Y軸の移動ヴェクトル

	bool isDeparture_ = false;						//エレベーターが出発しているかどうか

	VECTOR targetPos_ = {};							//現在向かっているポジション

	std::shared_ptr<Switch> switch_;				//スイッチクラスのスマートポインタ

	std::vector<std::shared_ptr<Lever>> levers_;	//レバークラスのスマートポインタのVector配列
};

