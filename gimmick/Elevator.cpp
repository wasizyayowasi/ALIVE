#include "Elevator.h"
#include "Switch.h"
#include "Lever.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

namespace {
	//エレベーターが移動する速度
	constexpr float move_speed = 6.0f;

	//出発する時間
	constexpr int max_departure_time = 60;

	//到達時間
	constexpr float total_time = 180.0f;
}

//コンストラクタ
Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//ポジションの初期化
	pos_ = objInfo.pos;

	//ターゲットポジションの初期化
	targetPos_ = pos_;

	//衝突判定用フレームの設定
	model_->SetUseCollision(true, false);

	//このエレベーターが何番目のエレベーターかを名前の末尾から取得する
	int elevatorNum = StrUtil::GetNumberFromString(objInfo.name, ".");

	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string switchName = StrUtil::GetConcatenateNumAndStrings("ElevatorSwitch", ".", elevatorNum);

	//上記で連結した名前を持つデータを取得してインスタンス化する
	switch_ = std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, switchName.c_str()));

	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string pointName = StrUtil::GetConcatenateNumAndStrings("ElevatorPoint", ".", elevatorNum);

	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string leverName = StrUtil::GetConcatenateNumAndStrings("ElevatorLever", ".", elevatorNum);

	//レバーのインスタンス化
	for (int i = 0; i < 2; i++) {
		std::string specificPointName = StrUtil::GetConcatenateNumAndStrings(pointName, "-", i);
		std::string specificLeverName = StrUtil::GetConcatenateNumAndStrings(leverName, "-", i);
		VECTOR stopPoint = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, specificPointName.c_str()).pos;
		levers_.push_back(std::make_shared<Lever>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, specificLeverName.c_str()), stopPoint));
	}

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false);
}

//デストラクタ
Elevator::~Elevator()
{
}

//更新
void Elevator::Update(Player& player)
{
	//プレイヤーのポジションの取得
	VECTOR playerPos = player.GetStatus().pos;

	//モデルの更新
	model_->Update();

	//スイッチの更新
	switch_->ChangeDuringStartup(elapsedTime_);
	switch_->Update(player);

	//レバーの更新
	for (auto lever : levers_) {
		lever->Update();
	}

	//レバーとプレイヤーの衝突判定を行い
	//衝突していた場合プレイヤーにポインタを設定する
	for (auto lever : levers_) {
		if (lever->CollCheck(playerPos)) {
			player.SetLeverPointer(lever);
		}
	}

	//ターゲットポジションの変更
	if (targetPos_.y == pos_.y && !isDeparture_) {
		TargetPosition();
	}
	else {
		if (!switch_->ElevatorCollResult()) {
			isDeparture_ = false;
		}
		switch_->DeleteHitResult();
	}

	//アニメーションが終了次第、移動を開始する
	if (model_->IsAnimEnd()) {
		Move();
	}
}

//描画
void Elevator::Draw()
{
	//モデルの描画
	model_->Draw();

	//スイッチの描画
	switch_->Draw();

	//レバーの描画
	for (auto lever : levers_) {
		lever->Draw();
	}
}

//エレベーターを移動させる
void Elevator::Move()
{
	//時間の更新
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//座標の移動
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, targetPos_.y, pos_.y);

	//移動終了後アニメーションを変更する
	if (elapsedTime_ == 180.0f) {
		SoundManager::GetInstance().Play("door");
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::open), false, false, 10);
	}

	//ポジションの設定
	model_->SetPos(pos_);

	//スイッチのポジションの設定
	switch_->GetModelPointer()->SetPos(VGet(pos_.x, pos_.y + 8.0f, pos_.z));
}

//移動先のポジションを取得する
void Elevator::TargetPosition()
{
	float distanceSize = {};
	float maxSize = 0.0f;

	//一番遠いレバーが所持しているポジションを取得する
	if (switch_->ElevatorCollResult()) {
		for (auto lever : levers_) {
			VECTOR stopPos = lever->GetElevatorStopPoint();
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos);
			if (maxSize < distanceSize) {
				maxSize = distanceSize;
				targetPos_ = stopPos;
				SoundManager::GetInstance().Play("door");
				model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
				elapsedTime_ = 0;
			}
		}
		isDeparture_ = true;
	}

	//レバーが引かれたらアニメーションを変更して
	//目的地まで移動する
	if (levers_.front()->IsOn()) {
		float size = MathUtil::GetSizeOfDistanceTwoPoints(targetPos_, levers_.front()->GetElevatorStopPoint());
		if (size != 0.0f) {
			targetPos_ = levers_.front()->GetElevatorStopPoint();
		}
		else {
			targetPos_ = levers_.back()->GetElevatorStopPoint();
		}
		SoundManager::GetInstance().Play("door");
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
		elapsedTime_ = 0;
	}

	//レバーが引かれたらアニメーションを変更して
	//目的地まで移動する
	if (levers_.back()->IsOn()) {
		float size = MathUtil::GetSizeOfDistanceTwoPoints(targetPos_, levers_.back()->GetElevatorStopPoint());
		if (size != 0.0f) {
			targetPos_ = levers_.back()->GetElevatorStopPoint();
		}
		else {
			targetPos_ = levers_.front()->GetElevatorStopPoint();
		}
		SoundManager::GetInstance().Play("door");
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
		elapsedTime_ = 0;
	}
}

//衝突判定を行うモデルの追加
std::shared_ptr<Model> Elevator::AddCollModel()
{
	return switch_->GetModelPointer();
}
