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
Elevator::Elevator(int handle,Material materialType, LoadObjectInfo objInfo):GimmickBase(handle, materialType, objInfo)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

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
	switch_ = std::make_shared<Switch>(file.GetSpecifiedGimmickInfo(pos_, switchName.c_str()));

	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string pointName = StrUtil::GetConcatenateNumAndStrings("ElevatorPoint", ".", elevatorNum);

	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string leverName = StrUtil::GetConcatenateNumAndStrings("ElevatorLever", ".", elevatorNum);

	//レバーのインスタンス化
	for (int i = 0; i < 2; i++)
	{
		std::string specificLeverName = StrUtil::GetConcatenateNumAndStrings(leverName, "-", i);
		levers_.push_back(std::make_shared<Lever>(file.GetSpecifiedGimmickInfo(pos_, specificLeverName.c_str())));
	}

	//停止ポジションの取得
	for (int i = 0; i < 2; i++)
	{
		std::string specificPointName = StrUtil::GetConcatenateNumAndStrings(pointName, "-", i);
		stopPos_[static_cast<ElevatorState>(i)] = file.GetSpecifiedGimmickInfo(pos_, specificPointName.c_str()).pos;
	}

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false);

	//停止ポジションとの距離を取得
	float upperDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::upper]);
	float lowerDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::lower]);

	//現在のポジションと停止ポジションとの距離が近い方の状態を保存する
	if (upperDistanceSize > lowerDistanceSize) {
		state_ = ElevatorState::lower;
	}
	else
	{
		state_ = ElevatorState::upper;
	}
}

//デストラクタ
Elevator::~Elevator()
{
}

//更新
void Elevator::Update(Player& player)
{
	//短縮化
 	auto& sound = SoundManager::GetInstance();

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
	if (targetPos_.y == pos_.y && !isDeparture_ && !isOnSwitch_) {
		TargetPosition();
	}
	else {
		if (!switch_->ElevatorCollResult()) {
			isOnSwitch_ = false;
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
	if (elapsedTime_ == 180.0f && isDeparture_) {
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::open), false, false, 10);
		SoundManager::GetInstance().Set3DSoundInfo(pos_, 1500.0f, "door");
		SoundManager::GetInstance().PlaySE("door");
		isDeparture_ = false;
	}

	if (model_->IsAnimEnd() && model_->GetCurrentAnimNo() == static_cast<int>(ElevatorAnimType::open)) {
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false, 10);
	}

	//ポジションの設定
	model_->SetPos(pos_);

	//スイッチのポジションの設定
	switch_->GetModelPointer()->SetPos(VGet(pos_.x, pos_.y + 8.0f, pos_.z));
}

//移動先のポジションを取得する
void Elevator::TargetPosition()
{
	//一番遠いレバーが所持しているポジションを取得する
	if (switch_->ElevatorCollResult() && !isOnSwitch_)
	{
		isOnSwitch_ = true;
		isDeparture_ = true;
	}

	//レバーが引かれたらアニメーションを変更して
	//目的地まで移動する
	if (levers_.front()->IsOn()) {
		isDeparture_ = true;
	}

	//レバーが引かれたらアニメーションを変更して
	//目的地まで移動する
	if (levers_.back()->IsOn()) {
		isDeparture_ = true;
	}

	if (isDeparture_)
	{
		//サウンドを鳴らす
		SoundManager::GetInstance().Set3DSoundInfo(pos_, 1500.0f, "door");
		SoundManager::GetInstance().PlaySE("door");

		//経過時間を0にする
		elapsedTime_ = 0;

		//アニメーションを変更
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
	}

	if (isDeparture_)
	{
		//elevatorの状態によって
		//elevatorの停止ポジションを変更する
		switch (state_)
		{
		case ElevatorState::upper:
			targetPos_ = stopPos_[ElevatorState::lower];
			state_ = ElevatorState::lower;
			break;
		case ElevatorState::lower:
			targetPos_ = stopPos_[ElevatorState::upper];
			state_ = ElevatorState::upper;
			break;
		}
	}
}

//衝突判定を行うモデルの追加
std::shared_ptr<Model> Elevator::AddCollModel()
{
	return switch_->GetModelPointer();
}
