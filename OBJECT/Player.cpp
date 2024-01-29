#include "Player.h"

#include "../object/DeadPerson.h"
#include "../object/ObjectManager.h"

#include "../gimmick/ManualCrank.h"
#include "../gimmick/Lever.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

#include<algorithm>
#include<string>

namespace {
	//重力
	constexpr float gravity = -0.4f;

	//フレームの名前
	const char* const frame_name = "hand.R_end";

	//プレイヤーの高さ
	constexpr float player_hegiht = 130.0f;
}

using namespace std;

Player::Player():updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObjectUpdate)
{
	//ジャンプ情報の初期
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//プレイヤーの高さの設定
	status_.height = player_hegiht;
	deathCount_ = 3;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

void Player::Init(LoadObjectInfo info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	//プレイヤー情報の初期化
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();
	
	//プレイヤーモデルの生成
	model_ = make_shared<Model>(model.GetModelHandle(ObjectType::Player));

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//プレイヤーの大きさの調整
	model_->SetScale(info.scale);

	//ポジションの設定
	model_->SetPos(info.pos);
	status_.pos = info.pos;
//	status_.pos = VGet(0,41,0);

	//回転率の設定
	model_->SetRot(info.rot);

	//コリジョンフレームの設定
	model_->SetUseCollision(true,true,"Character");

	scale_ = info.scale;
}

void Player::Update(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//プレイヤーのアニメーション更新
	model_->Update();
	
	(this->*updateFunc_)(input,objManager);
}


void Player::Draw()
{
	//モデルの描画
	model_->Draw();

	//影もどきの描画
	DrawPolygon3D();

	DrawFormatString(0, 64, 0xffffff, "normal %.2f,%.2f,%.2f", status_.pos.x,status_.pos.y,status_.pos.z);
}

void Player::SetPos(VECTOR pos)
{
	status_.pos = pos;
	model_->SetPos(status_.pos);
}

void Player::SetJumpInfo(bool isJump, float jumpVec)
{
	status_.jump.isJump = isJump;
	status_.jump.jumpVec = jumpVec;
}

//HACK:↓汚い、気に食わない
/// <summary>
/// アニメーションがidle状態の時に行われる
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::NormalUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//削除予定
	if (input.IsTriggered(InputType::creative)) {
		debugCreativeMode = !debugCreativeMode;
	}
	
	//オブジェクトに対してアクションを起こす
	if (input.IsTriggered(InputType::activate)) {
		status_.moveVec = VGet(0, 0, 0);
		if (status_.situation.isCanBeCarried) {
			(this->*carryUpdateFunc_)();
		}
		else if (status_.situation.isGimmickCanBeOperated) {
			if (crank_ != nullptr) {
				//クランクを動かす準備をする
				updateFunc_ = &Player::GoCrankRotationPosition;
			}
			else if (lever_ != nullptr) {
				//レバーを動かす準備をする
				updateFunc_ = &Player::GoLeverPullPosition;
			}
			return;
		}
	}
	else {
		crank_.reset();
		lever_.reset();
	}

	if (input.IsTriggered(InputType::sit)) {
		updateFunc_ = &Player::IdleToSitup;
		return;
	}

	status_.situation.isGimmickCanBeOperated = false;

	if (status_.situation.isInTransit) {
		//dynamic_pointer_cast<DeadPerson>(deadPersonModelPointer_)->Init();
		deadPersonModelPointer_->GetModelPointer()->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		VECTOR framePos = model_->GetFrameLocalPosition(frame_name);
		deadPersonModelPointer_->GetModelPointer()->SetPos(framePos);
	}
	else {
		status_.situation.isCanBeCarried = false;
	}

	ChangeAnimIdle();
	MovingUpdate(input);

	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (!debugCreativeMode) {
		if (status_.jump.isJump) {
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
			model_->SetPos(status_.pos);
		}
		else {
			status_.jump.jumpVec = 0.0f;
		}
	}
	

	//持ち運び中だったら
	//以降の処理を行わない
	if (status_.situation.isInTransit) {
		return;
	}

	//メンバ関数ポインタをrunningJumpUpdate、
	//jumpUpdateのどちらかに変更する
	if (!debugCreativeMode) {
		if (input.IsTriggered(InputType::space)) {
			if (!status_.jump.isJump) {
				PlayerJump(playerInfo_.jumpPower);
			}
			ChangeAnimNo(PlayerAnimType::jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::space)) {
			status_.moveVec.y = 10.0;
		}
	}

	//メンバ関数ポインタをDeathUpdateに変更する
	if (input.IsTriggered(InputType::death)) {
		deathCount_++;
		updateFunc_ = &Player::DeathUpdate;
		status_.moveVec = VGet(0, 0, 0);
		return;
	}

}

/// <summary>
/// アニメーションをidleに戻す関数
/// </summary>
void Player::ChangeAnimIdle()
{
	//待機アニメーションに戻す
	if (!status_.situation.isMoving) {
		ChangeAnimNo(PlayerAnimType::Idle, true, 20);
	}
}

//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーを移動させるための関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::MovingUpdate(const InputState& input)
{
	
	float movingSpeed = Move(input);
	
	//HACK：もっといいアニメーション番号変更があるはず
	if (movingSpeed != 0.0f) {
		if (movingSpeed > playerInfo_.walkSpeed) {
			//アニメーションの変更
			ChangeAnimNo(PlayerAnimType::Run, true, 20);
		}
		else if (movingSpeed <= playerInfo_.walkSpeed) {
			//アニメーションの変更
			ChangeAnimNo(PlayerAnimType::Walk, true, 20);
		}
	}

	if (VSize(status_.moveVec) == 0.0f) {
		status_.situation.isMoving = false;
		return;
	}

	//移動ベクトルを用意する
	status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);
}

float Player::Move(const InputState& input) {
	//キーの押下をブール型に格納
	bool pressedUp = input.IsPressed(InputType::up);
	bool pressedDown = input.IsPressed(InputType::down);
	bool pressedLeft = input.IsPressed(InputType::left);
	bool pressedRight = input.IsPressed(InputType::right);
	bool pressedShift = input.IsPressed(InputType::dush);

	status_.moveVec = VGet(0,0,0);

	status_.situation.isMoving = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = PlayerSpeed(pressedShift);
		status_.situation.isMoving = true;
	}

	//HACK：汚い、リファクタリング必須
	if (pressedUp) {
		status_.moveVec.z = movingSpeed;
		targetAngle_ = 180.0f;
	}
	if (pressedDown) {
		status_.moveVec.z = -movingSpeed;
		targetAngle_ = 0.0f;
	}
	if (pressedLeft) {
		status_.moveVec.x = -movingSpeed;
		targetAngle_ = 90.0f;
	}
	if (pressedRight) {
		status_.moveVec.x = movingSpeed;
		targetAngle_ = 270.0f;
	}
	if (pressedUp && pressedRight) {
		targetAngle_ = 225.0f;
	}
	if (pressedUp && pressedLeft) {
		targetAngle_ = 135.0f;
	}
	if (pressedDown && pressedLeft) {
		targetAngle_ = 45.0f;
	}
	if (pressedDown && pressedRight) {
		targetAngle_ = 315.0f;
	}

	//回転処理
	RotationUpdate();

	VECTOR destination = VAdd(status_.pos, status_.moveVec);
	if (destination.z < -250.0f) {
		status_.moveVec.z = 0.0f;
	}

	return movingSpeed;

}

//完成品だから今後いじらなくていいと思う
/// <summary>
/// プレイヤーの回転処理を行う関数
/// </summary>
void Player::RotationUpdate()
{
	//一回転の角度
	float oneRevolution = 360.0f;

	//目標の角度から現在の角度を引いて差を出している
	differenceAngle_ = targetAngle_ - angle_;

	//常にプレイヤーモデルを大周りさせたくないので
	//181度又は-181度以上の場合、逆回りにしてあげる
	if (differenceAngle_ >= oneRevolution / 2) {
		differenceAngle_ = targetAngle_ - angle_ - oneRevolution;
	}
	else if (differenceAngle_ <= -oneRevolution / 2) {
		differenceAngle_ = targetAngle_ - angle_ + oneRevolution;
	}

	//滑らかに回転させるため
	//現在の角度に回転スピードを増減させている
	if (differenceAngle_ < 0.0f) {
		status_.rot.y -= playerInfo_.rotSpeed;
		angle_ -= playerInfo_.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) {
		status_.rot.y += playerInfo_.rotSpeed;
		angle_ += playerInfo_.rotSpeed;
	}

	//360度、一周したら0度に戻すようにしている
	if (angle_ == oneRevolution || angle_ == -oneRevolution) {
		angle_ = 0.0f;
	}
	if (status_.rot.y == oneRevolution || status_.rot.y == -oneRevolution) {
		status_.rot.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
}

//HACK:↓汚い、気に食わない
/// 走りジャンプではないときのジャンプ
void Player::JumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//プレイヤー移動関数
	Move(input);

	//ジャンプ処理
	{
		//ジャンプベクトルが0でジャンプ中ではなかったら
		//idle状態のアップデートに変更する、アニメーションも変更する
		if (!status_.jump.isJump) {
			updateFunc_ = &Player::NormalUpdate;
			return;
		}

		status_.jump.jumpVec += gravity;
		status_.pos.y += status_.jump.jumpVec;
		model_->SetPos(status_.pos);

	}
}

// プレイヤーの死体に与える情報を作る関数
void Player::DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//座るアニメーション以外だったら死ぬアニメーションに変える
	if (status_.animNo != static_cast<int>(PlayerAnimType::idleToSitup)) {
		//アニメーションの変更
		ChangeAnimNo(PlayerAnimType::Death, false, 20);
	}

	if (model_->IsAnimEnd()) {
		DeathPersonPostProsessing(objManager);
	}

}

//死体の後処理
void Player::DeathPersonPostProsessing(std::shared_ptr<ObjectManager> objManager)
{
//	status_.pos = checkPoint_;				//チェックポイントにプレイヤーを帰す

	DeadPersonGenerater(objManager);			//死体を生成する関数

	updateFunc_ = &Player::NormalUpdate;
}

// プレイヤーの死体をvector配列で管理する関数
void Player::DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager)
{
	LoadObjectInfo info;
	info.rot = MathUtil::VECTORDegreeToRadian(status_.rot);
	info.scale = scale_;

	int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
	VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
	info.pos = putPos;

	objManager->DeadPersonGenerator(model_->GetModelHandle(),info, status_.animNo);
}

// プレイヤーに座るアニメーションをさせる関数
void Player::SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//立ち上がるためのコマンド
	if (input.IsTriggered(InputType::activate)) {
		//アニメーションの変更
		ChangeAnimNo(PlayerAnimType::situpToIdle, false, 20);
	}
	
	//立つ家庭のアニメーションが終わったらidleupdateに変更する
	if (status_.animNo == static_cast<int>(PlayerAnimType::situpToIdle) && model_->IsAnimEnd()) {
		updateFunc_ = &Player::NormalUpdate;
		status_.situation.isSitting = false;
		return;
	}

	if (status_.animNo == static_cast<int>(PlayerAnimType::situpToIdle)) return;

	//死ぬコマンド
	if (input.IsTriggered(InputType::death)) {
		DeathUpdate(input,objManager);
		status_.situation.isSitting = false;
		updateFunc_ = &Player::NormalUpdate;
		return;
	}

}

void Player::IdleToSitup(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{

	status_.moveVec = VGet(0.0f, 0.0f, 0.0f);

	//アニメーションを座る過程のアニメーションに変更
	//座っているフラグを立て、アニメーションループ変数を折る
	if (!status_.situation.isSitting) {
		status_.situation.isSitting = true;
		ChangeAnimNo(PlayerAnimType::idleToSitup, false, 20);
	}

	//座る過程のアニメーションが終わったら三角座りにする
	if (status_.animNo == static_cast<int>(PlayerAnimType::idleToSitup) && model_->IsAnimEnd()) {
		model_->SetAnimEndFrame(status_.animNo);
		updateFunc_ = &Player::SitUpdate;
	}

}

void Player::SetCarryInfo(bool isCarry, shared_ptr<ObjectBase>model) {
	status_.situation.isCanBeCarried = isCarry;
	deadPersonModelPointer_ = model;
}

void Player::SetCrankPointer(std::shared_ptr<ManualCrank> crank) {
	crank_ = crank;
	status_.situation.isGimmickCanBeOperated = true;
}

void Player::SetLeverPointer(std::shared_ptr<Lever> lever)
{
	lever_ = lever;
	status_.situation.isGimmickCanBeOperated = true;
}

void Player::CarryObjectUpdate()
{
	
	if (!status_.situation.isCanBeCarried) return;

	status_.situation.isInTransit = true;

	deadPersonModelPointer_->SetIsTransit(true);

	carryUpdateFunc_ = &Player::DropOffObjectUpdate;

}

//運んでいたオブジェクトを下ろす処理
void Player::DropOffObjectUpdate()
{
	if (status_.situation.isCanBeCarried) {
		status_.situation.isCanBeCarried = false;
		deadPersonModelPointer_->SetIsTransit(false);
		int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
		VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
		deadPersonModelPointer_->GetModelPointer()->SetPos(putPos);
		deadPersonModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObjectUpdate;

}

void Player::CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager) 
{

	status_.moveVec = VGet(0, 0, 0);

	float rotZ = crank_->GetRotZ();

	if (input.IsPressed(InputType::down)) {
		rotZ = (std::max)(rotZ - 3.0f, crank_->GetMaxRotZ());
		CrankRotationUpdate(rotZ);
	}
	else if (input.IsPressed(InputType::up)) {
		rotZ = (std::min)(rotZ + 3.0f, 0.0f);
		CrankRotationUpdate(rotZ);
	}

	int naturalNumber = (std::max)(rotZ, -rotZ);
	float animTime = static_cast<float>(naturalNumber % 360) / 3;

	model_->SetAnimationFrame(animTime);

	if (input.IsTriggered(InputType::activate)) {
		status_.situation.isGimmickCanBeOperated = false;
		crank_.reset();
		updateFunc_ = &Player::NormalUpdate;
	}

}

void Player::CrankRotationUpdate(float rotZ) {

	float radian = MathUtil::DegreeToRadian(rotZ);

	int frameNo = MV1SearchFrame(crank_->GetModelPointer()->GetModelHandle(), "Crank");

	VECTOR pos = MV1GetFramePosition(crank_->GetModelPointer()->GetModelHandle(), frameNo);
	//クランクのポジションからオブジェクト全体のポジションを引いた距離
	VECTOR distance = { 0,1.8f,0 };

	MATRIX mat = {};

	float x = MathUtil::DegreeToRadian(90.0f);

	//平行移動行列
	MATRIX posMat = MGetTranslate(distance);
	//回転行列
	MATRIX rotMatZ = MGetRotZ(radian);
	MATRIX rotMatX = MGetRotX(x);

	mat = MMult(rotMatX, rotMatZ);
	mat = MMult(mat, posMat);

	MV1SetFrameUserLocalMatrix(crank_->GetModelPointer()->GetModelHandle(), frameNo, mat);

	crank_->SetRotZ(rotZ);

}

void Player::BulletHitMeUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//重力
	status_.jump.jumpVec += gravity;
	status_.moveVec.y = status_.jump.jumpVec;

	//ノックバック
	status_.moveVec = VScale(status_.moveVec, 0.96f);

	status_.pos = VAdd(status_.pos, status_.moveVec);

	model_->SetPos(status_.pos);

	float moveVecSize = VSize(status_.moveVec);

	if (moveVecSize < 2.0f) {
		updateFunc_ = &Player::NormalUpdate;
	}

}

void Player::GoLeverPullPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//クランクの立ってほしいポジションを取得する
	VECTOR standPos = lever_->GetStandingPosition();
	//立ってほしいポジションとプレイヤーの距離のサイズを取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSizeが一定の範囲外だったら
	//一定の速度で立ってほしいポジションに向かう
	if (distanceSize > 3.0f) {
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSizeが一定の範囲内に入ったら
	//立ってほしいポジションをプレイヤーのポジションとする
	else {
		model_->SetPos(standPos);
		angle_ = 0.0f;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::LeverOn, false, 10);
		lever_->OnAnimation();
		updateFunc_ = &Player::LeverUpdate;
	}
}

void Player::LeverUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	if (!lever_->IsOn()) {
		updateFunc_ = &Player::NormalUpdate;
	}
}

//クランクを回すためにクランクを回すポジションへと移動する
void Player::GoCrankRotationPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//クランクの立ってほしいポジションを取得する
	VECTOR standPos = crank_->GetStandingPosition();
	//立ってほしいポジションとプレイヤーの距離のサイズを取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSizeが一定の範囲外だったら
	//一定の速度で立ってほしいポジションに向かう
	if (distanceSize > 3.0f) {
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSizeが一定の範囲内に入ったら
	//立ってほしいポジションをプレイヤーのポジションとする
	else {
		model_->SetPos(standPos);
		angle_ = -90.0f;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::crank, false, 20);
		updateFunc_ = &Player::CrankUpdate;
	}

}

void Player::BulletHitMe(VECTOR moveVec)
{
	status_.moveVec = moveVec;

	updateFunc_ = &Player::BulletHitMeUpdate;
}

void Player::ChangeAnimNo(PlayerAnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = static_cast<int>(type);
	status_.isAnimLoop = isAnimLoop;
	model_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//プレイヤーの速度設定
float Player::PlayerSpeed(bool pressedShift)
{
	//シフトが押されているかどうか
	if (pressedShift) {
		if (debugCreativeMode) {
			return playerInfo_.runningSpeed * 3;
		}
		return playerInfo_.runningSpeed;
	} 
	
	return playerInfo_.walkSpeed;
}

//ジャンプの設定
void Player::PlayerJump(float jumpPower) {
	status_.jump.jumpVec = jumpPower;
	status_.pos.y += status_.jump.jumpVec;
	status_.jump.isJump = true;
}

//落ち影を作成、描画
void Player::DrawPolygon3D()
{
	//頂点の数分配列を作る
	VERTEX3D vertex[7] = {};
	//三角形を作成する順番を保存する配列
	WORD index[18] = {};

	//カラー
	COLOR_U8 difColor = GetColorU8(51, 51, 51, 125);
	COLOR_U8 spcColor = GetColorU8(0, 0, 0, 0);

	//法線ベクトル
	VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

	//角度に寄って頂点の位置を変更する
	float angle = 0.0f;

	//六角形の中心座標を取得
	vertex[0].pos = VGet(status_.pos.x, roundShadowHeight_, status_.pos.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//角度ごとの頂点を取得
	for (int i = 1; i < 7; i++) {
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = norm;
		vertex[i].dif = difColor;
		vertex[i].spc = spcColor;
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 60.0f;
	}

	//三角形を作成する順番
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	index[6] = 0;
	index[7] = 3;
	index[8] = 4;
	index[9] = 0;
	index[10] = 4;
	index[11] = 5;
	index[12] = 0;
	index[13] = 5;
	index[14] = 6;
	index[15] = 0;
	index[16] = 6;
	index[17] = 1;

	DrawPolygonIndexed3D(vertex, 7, index, 6, DX_NONE_GRAPH, true);
}

//プレイヤーの落ち影に使用する頂点を取得
VECTOR Player::VertexPosition(float angle)
{

	VECTOR pos = {};

	//度数法を弧度法に変換する
	float radian = MathUtil::DegreeToRadian(angle);
	
	//角度による座標を取得する
	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	//ポジションを25倍する(サイズ調整)
	pos = VScale(pos, 25.0f);

	//プレイヤーのポジションと上記で取得したポジションを足す
	pos = VAdd(status_.pos, pos);

	//Y座標は線とポリゴンの当たり判定で取得した
	//一番近いポリゴンのY座標を代入する
	pos.y = roundShadowHeight_;

	return pos;
}
