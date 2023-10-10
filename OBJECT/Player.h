#pragma once
#include "DxLib.h"
#include "CharacterBase.h"
#include "util/ObjAnimType.h"
#include <list>
#include <unordered_map>
#include <memory>

class InputState;
class Model;
class CheckCollisionModel;
class GimmickBase;

struct JumpInfo {
	float jumpVec;		//ジャンプベクトル
	bool isJump;		//ジャンプ中かどうか
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int anim_[static_cast<int>(AnimType::max)];
};

struct PlayerStatus {
	JumpInfo jump;
	VECTOR pos;
	VECTOR rot;
	VECTOR moveVec;
	int animNo;
	float height;
	bool isAnimLoop;
	bool isCarry;
};

class Player
{
public:

	Player(const char* const filename);
	Player(int handle);
	virtual ~Player();

	void Init();

	/// <summary>
	/// プレイヤーの更新を行う
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	/// <param name="models">衝突判定を行うモデルのvector型の配列</param>
	void Update(const InputState& input);

	/// <summary>
	/// プレイヤー関連の描画
	/// </summary>
	void Draw();

	VECTOR GetRot();

	/// <summary>
	/// 外部からのポジションを受け取る
	/// </summary>
	/// <param name="pos">ポジション情報</param>
	void SetPos(VECTOR pos);

	/// <summary>
	/// 外部からのジャンプ情報を受け取る
	/// </summary>
	/// <param name="isJump">ジャンプフラグ</param>
	/// <param name="jumpVec">ジャンプベクトル</param>
	void SetJumpInfo(bool isJump, float jumpVec);

	void SetClim(bool isClim) { isClim_ = isClim; }
	void SetCarryInfo(bool isCarry, std::shared_ptr<Model> model);

	void SetSaveData(VECTOR pos, int num, bool isContinue);
	int GetDeathNum() {return deathCount_;	}

	PlayerStatus GetStatus() { return status_; }

	std::shared_ptr<Model> GetTempCustodyPointer() {return temporaryCustodyPointer_;}
		
private:
	//通常更新
	void IdleUpdate(const InputState& input);

	//待機処理
	void ChangeAnimIdle();

	//移動処理
	void MovingUpdate(const InputState& input);

	//回転処理
	void RotationUpdate();

	//オブジェクトを登る
	void ClimUpdate(const InputState& input);

	//ジャンプ処理
	void JumpUpdate(const InputState& input);

	//走りジャンプ処理
	void RunningJumpUpdate(const InputState& input);

	//死亡処理
	void DeathUpdate(const InputState& input);

	//死体の後処理
	void DeathPersonPostProsessing();

	//死人生成
	void DeadPersonGenerater();

	//座る
	void SitUpdate(const InputState& input);

	//座っている途中
	void IdleToSitup(const InputState& input);

	//立ち上がる
	void StandUpdate(const InputState& input);

	//持ち運ぶ
	void CarryObjectUpdate(const InputState& input);

private:
	/// <summary>
	/// プレイヤーの移動速度を設定する
	/// </summary>
	/// <returns>float型の移動速度</returns>
	float PlayerSpeed(bool pressedShift);


	VECTOR FramPosition(const char* const LeftFramename, const char* const RightFramename);

private:

	float temp = 0;
	float tempGravity = 0.0f;

	int deathCount_ = 0;

	float targetAngle_ = 0.0f;				//回転
	float differenceAngle_ = 0.0f;			//目標の角度と現在の角度の差
	float tempAngle_ = 0.0f;

	bool isMoving_ = false;					//移動中か
	bool isSitting_ = false;				//座っているか
	bool isClim_ = false;
	bool isContinue_ = false;				//これだめだと思う

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//中間ポイント

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//死体のポジション

	std::shared_ptr<Model> temporaryCustodyPointer_;
	std::shared_ptr<Model> PModel_;								//モデルクラスのポインタ
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ

	std::unordered_map<AnimType, int> animType_;				

	void(Player::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ
};

