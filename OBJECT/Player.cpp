#include "Player.h"
#include "../gimmick/ManualCrank.h"
#include "../util/InputState.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ObjectManager.h"
#include<algorithm>
#include<string>

namespace {
	//重力
	constexpr float gravity = -0.4f;

	//ファイルパス
	const char* const player_Filename = "data/player/temp.mv1";

	//フレームの名前
	const char* const frame_name = "foot.L";

	//プレイヤーの高さ
	constexpr float player_hegiht = 130.0f;

}

using namespace std;

Player::Player(const char* const filename):updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObjectUpdate)
{

	//ジャンプ情報の初期
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//プレイヤーの高さの設定
	status_.height = player_hegiht;
}

Player::Player(int handle) :updateFunc_(&Player::NormalUpdate)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

void Player::Init(LoadObjectInfo info)
{

	//プレイヤー情報の初期化
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();
	
	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo_.animNo_[i];
	}

	//プレイヤーモデルの生成
	model_ = make_shared<Model>(player_Filename);
	//アニメーションの設定
	model_->SetAnimation(animType_[AnimType::idle], true, false);
	//プレイヤーの大きさの調整
	model_->SetScale(info.scale);
	//ポジションの設定
	model_->SetPos(info.pos);
	status_.pos = info.pos;
	//回転率の設定
	model_->SetRot(info.rot);
	//コリジョンフレームの設定
	model_->SetCollFrame("Character");

	scale_ = info.scale;

}

void Player::Update(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//移動ベクトルのリセット
	//status_.moveVec = { 0.0f,0.0f,0.0f };

	//プレイヤーのアニメーション更新
	model_->Update();
	
	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	(this->*updateFunc_)(input,objManager);

}


void Player::Draw()
{
	model_->Draw();

	DrawPolygon3D();

//	float au = 20.0f;

//	DrawLine3D(status_.pos, VAdd(status_.pos, VGet(0, status_.height, 0)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet( au, 0,   0)), VAdd(status_.pos, VGet( au, status_.height,   0)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet(-au, 0,   0)), VAdd(status_.pos, VGet(-au, status_.height,   0)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet(  0, 0,  au)), VAdd(status_.pos, VGet(  0, status_.height,  au)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet(  0, 0, -au)), VAdd(status_.pos, VGet(  0, status_.height, -au)), 0xff0000);

	//DrawSphere3D(VGet(status_.pos.x + 1000, status_.pos.y, status_.pos.z), 16, 32, 0xff0000, 0xff0000, true);

	DrawFormatString(0, 64, 0x448844, "%.2f,%.2f,%.2f", status_.pos.x,status_.pos.y,status_.pos.z);

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

	if (input.IsTriggered(InputType::creative)) {
		debugCreativeMode = !debugCreativeMode;
	}

	if (input.IsTriggered(InputType::activate)) {
		if (status_.situation.isCanBeCarried) {
			(this->*carryUpdateFunc_)();
		}
		else if (status_.situation.isGimmickCanBeOperated) {
			updateFunc_ = &Player::CrankUpdate;
			return;
		}
		else {
			updateFunc_ = &Player::IdleToSitup;
			return;
		}
	}

	status_.situation.isGimmickCanBeOperated = false;

	if (status_.situation.isInTransit) {
		deadPersonModelPointer_->GetModelPointer()->SetAnimEndFrame(animType_[AnimType::dead]);
		deadPersonModelPointer_->GetModelPointer()->SetRot(DegreesToRadians(status_.rot));
		deadPersonModelPointer_->GetModelPointer()->SetPos(FramPosition("hand.R_end"));
	}
	else {
		status_.situation.isCanBeCarried = false;
	}

	ChangeAnimIdle();
	MovingUpdate(input,objManager);

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
			ChangeAnimNo(AnimType::jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::space)) {
			status_.moveVec.y = 3.0;
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

		if (status_.situation.isInTransit) {
			ChangeAnimNo(AnimType::carryIdle, true, 20);
		}
		else {
			ChangeAnimNo(AnimType::idle, true, 20);
		}
	}
}

//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーを移動させるための関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::MovingUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	
	float movingSpeed = Move(input);

	//HACK：もっといいアニメーション番号変更があるはず
	if (movingSpeed != 0.0f) {
		if (movingSpeed > playerInfo_.walkSpeed) {
			//アニメーションの変更
			if (status_.situation.isInTransit) {
				ChangeAnimNo(AnimType::carryRunning, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::run, true, 20);
			}
		}
		else if (movingSpeed <= playerInfo_.walkSpeed) {
			//アニメーションの変更
			if (status_.situation.isInTransit) {
				ChangeAnimNo(AnimType::carryWalking, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::walk, true, 20);
			}
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
	bool pressedShift = input.IsPressed(InputType::shift);

	status_.moveVec = { 0.0f,0.0f,0.0f };

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

	return movingSpeed;

}

//完成品だから今後いじらなくていいと思う
/// <summary>
/// プレイヤーの回転処理を行う関数
/// </summary>
void Player::RotationUpdate()
{
	//目標の角度から現在の角度を引いて差を出している
	differenceAngle_ = targetAngle_ - angle_;

	//常にプレイヤーモデルを大周りさせたくないので
	//181度又は-181度以上の場合、逆回りにしてあげる
	if (differenceAngle_ >= 180.0f) {
		differenceAngle_ = targetAngle_ - angle_ - 360.0f;
	}
	else if (differenceAngle_ <= -180.0f) {
		differenceAngle_ = targetAngle_ - angle_ + 360.0f;
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
	if (angle_ == 360.0f || angle_ == -360.0f) {
		angle_ = 0.0f;
	}
	if (status_.rot.y == 360.0f || status_.rot.y == -360.0f) {
		status_.rot.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	model_->SetRot(DegreesToRadians(status_.rot));
}

//オブジェクトを登る
void Player::ClimUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	if (model_->IsAnimEnd()) {
		status_.pos = CenterFramPosition("mixamorig:LeftToeBase", "mixamorig:RightToeBase");
		model_->SetPos(status_.pos);

		status_.animNo = animType_[AnimType::stand];
		status_.isAnimLoop = false;
		model_->SetAnimation(status_.animNo, status_.isAnimLoop, true);
		updateFunc_ = &Player::StandUpdate;
	}
}

//HACK:↓汚い、気に食わない
/// <summary>
/// 走りジャンプではないときのジャンプ
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
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


//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーが走っているときのジャンプ
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::RunningJumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//プレイヤー移動関数
	Move(input);

	//HACK：変数が仮のまま　+　どうするか悩んでいる
	//アニメーションの総時間によって、重力を変更する
	totalAnimFrame_ = model_->GetAnimTotalTime() + 2;
	runJumpGravity = -(playerInfo_.runningJumpPower * 2 / totalAnimFrame_);

	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (status_.jump.isJump) {
		status_.jump.jumpVec += runJumpGravity;
		status_.pos.y += status_.jump.jumpVec;
	}

	//ジャンプベクトルが0でジャンプ中ではなかったら
	//idle状態のアップデートに変更する、アニメーションも変更する
	if (status_.jump.jumpVec == 0.0f && !status_.jump.isJump) {
		updateFunc_ = &Player::NormalUpdate;
		return;
	}
}

/// <summary>
/// プレイヤーの死体に与える情報を作る関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	deathPos_ = status_.pos;				//死んだ場所を残す

	//座るアニメーション以外だったら死ぬアニメーションに変える
	if (status_.animNo != animType_[AnimType::idleToSitup]) {
		//アニメーションの変更
		ChangeAnimNo(AnimType::death, false, 20);
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

/// <summary>
/// プレイヤーの死体をvector配列で管理する関数
/// </summary>
void Player::DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager)
{
	LoadObjectInfo info;
	info.rot = DegreesToRadians(status_.rot);
	info.pos = deathPos_;
	info.scale = scale_;

	objManager->DeadPersonGenerator(model_->GetModelHandle(),info, status_.animNo);
}

/// <summary>
/// プレイヤーに座るアニメーションをさせる関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//立ち上がるためのコマンド
	if (input.IsTriggered(InputType::activate)) {
		//アニメーションの変更
		ChangeAnimNo(AnimType::situpToIdle, false, 20);
	}
	
	//立つ家庭のアニメーションが終わったらidleupdateに変更する
	if (status_.animNo == animType_[AnimType::situpToIdle] && model_->IsAnimEnd()) {
		updateFunc_ = &Player::NormalUpdate;
		status_.situation.isSitting = false;
		return;
	}

	if (status_.animNo == animType_[AnimType::situpToIdle]) return;

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
		ChangeAnimNo(AnimType::idleToSitup, false, 20);
	}

	//座る過程のアニメーションが終わったら三角座りにする
	if (status_.animNo == animType_[AnimType::idleToSitup] && model_->IsAnimEnd()) {
		model_->SetAnimEndFrame(status_.animNo);
		updateFunc_ = &Player::SitUpdate;
	}

}

//立ち上がる処理
void Player::StandUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	if (model_->IsAnimEnd()) {
		updateFunc_ = &Player::NormalUpdate;
		status_.situation.isClim = false;
	}
}

void Player::SetCarryInfo(bool isCarry, shared_ptr<ObjectBase>model) {
	status_.situation.isCanBeCarried = isCarry;
	deadPersonModelPointer_ = model;
}

void Player::SetGimmickModelPointer(std::shared_ptr<ManualCrank> crank) {
	crank_ = crank;
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
	bool isCarryWalking = status_.animNo == animType_[AnimType::carryWalking];
	bool isCarry = status_.animNo == animType_[AnimType::carryIdle];
	if ((isCarryWalking || isCarry) && status_.situation.isCanBeCarried) {
		status_.situation.isCanBeCarried = false;
		deadPersonModelPointer_->SetIsTransit(false);
		deadPersonModelPointer_->GetModelPointer()->SetPos(CenterFramPosition("foot.L", "foot.R"));
		deadPersonModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObjectUpdate;

}

void Player::CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager) 
{

	status_.moveVec = VGet(0, 0, 0);

	//VECTOR pos = crank_->GetModelPointer()->GetPos();
	float rotZ = crank_->GetRotZ();

	if (input.IsPressed(InputType::down)) {
		rotZ = (std::max)(rotZ - 3.0f, -630.0f);
		CrankRotatinUpdate(rotZ);
	}
	else if (input.IsPressed(InputType::up)) {
		rotZ = (std::min)(rotZ + 3.0f, 0.0f);
		CrankRotatinUpdate(rotZ);
	}

	if (input.IsTriggered(InputType::activate)) {
		status_.situation.isGimmickCanBeOperated = false;
		crank_.reset();
		updateFunc_ = &Player::NormalUpdate;
	}

}

void Player::CrankRotatinUpdate(float rotZ) {

	float radian = rotZ * DX_PI_F / 180.0f;

	int frameNo = MV1SearchFrame(crank_->GetModelPointer()->GetModelHandle(), "Crank");

	VECTOR pos = MV1GetFramePosition(crank_->GetModelPointer()->GetModelHandle(), frameNo);
	//クランクのポジションからオブジェクト全体のポジションを引いた距離
	VECTOR distance = { 0,2.0f,0 };

	MATRIX mat = {};

	float x = 90 * DX_PI_F / 180.0f;

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

//度数法から弧度法に変換した角度を返す
VECTOR Player::DegreesToRadians(VECTOR rot)
{
	VECTOR storageRot;

	storageRot.x = rot.x * DX_PI_F / 180.0f;
	storageRot.y = rot.y * DX_PI_F / 180.0f;
	storageRot.z = rot.z * DX_PI_F / 180.0f;

	return storageRot;
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

void Player::BulletHitMe(VECTOR moveVec)
{
	status_.moveVec = moveVec;

	updateFunc_ = &Player::BulletHitMeUpdate;
}

void Player::ChangeAnimNo(AnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = animType_[type];
	status_.isAnimLoop = isAnimLoop;
	model_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//プレイヤーの速度設定
float Player::PlayerSpeed(bool pressedShift)
{
	if (pressedShift) {
		if (debugCreativeMode) {
			return playerInfo_.runningSpeed * 2;
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

//二つのフレーム座標の中心を取得する
VECTOR Player::CenterFramPosition(const char* const LeftFramename, const char* const RightFramename)
{

	VECTOR framePosition;

	//指定フレームの座標を取得する。
	framePosition = model_->GetFrameLocalPosition(LeftFramename);
	framePosition = VAdd(framePosition, model_->GetFrameLocalPosition(RightFramename));
	//二つの座標を足し、2で割り中心を取得する
	framePosition.x = framePosition.x / 2;
	framePosition.y = framePosition.y / 2;
	framePosition.z = framePosition.z / 2;

	return framePosition;
}

VECTOR Player::FramPosition(const char* const framename)
{

	VECTOR framePosition;

	//指定フレームの座標を取得する。
	framePosition = model_->GetFrameLocalPosition(framename);

	return framePosition;
}

void Player::DrawPolygon3D()
{
	VERTEX3D vertex[7];
	WORD index[18];

	float angle = 0.0f;

	vertex[0].pos = VGet(status_.pos.x, roundShadowHeight_, status_.pos.z);
	vertex[0].norm = VGet(0.0f, 1.0f, 0.0f);
	vertex[0].dif = GetColorU8(51, 51, 51, 125);
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	for (int i = 1; i < 7; i++) {
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
		vertex[i].dif = GetColorU8(51, 51, 51, 125);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 60.0f;

	}

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

VECTOR Player::VertexPosition(float angle)
{

	VECTOR pos = {};

	float radian = angle * DX_PI_F / 180.0f;

	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	pos = VScale(pos, 25.0f);

	pos = VAdd(status_.pos, pos);

	pos.y = roundShadowHeight_;

	return pos;
}
