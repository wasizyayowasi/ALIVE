#pragma once
#include "GimmickBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Switch;
class Lever;
class Model;

enum class ElevatorAnimType {
	openIdle,
	closeIdle,
	open,
	close,
	max,
};

class Elevator : public GimmickBase
{
public:
	//コンストラクタ
	Elevator(int handle, LoadObjectInfo objInfo);
	virtual ~Elevator();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	void Move();

	void TargetPosition();

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();
private:

	int departureTime_ = 0;

	float moveVecY_ = 0.0f;							//Y軸の移動ヴェクトル

	bool isDeparture_ = false;						//エレベーターが出発しているかどうか

	VECTOR targetPos_ = {};							//現在向かっているポジション

	std::shared_ptr<Switch> switch_;

	std::vector<std::shared_ptr<Lever>> levers_;
};

