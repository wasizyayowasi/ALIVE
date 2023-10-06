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
	bool isAnimLoop;
};

class Player
{
public:

	Player(const char* const filename);
	Player(int handle);
	virtual ~Player();

	void init();

	/// <summary>
	/// プレイヤーの更新を行う
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	/// <param name="models">衝突判定を行うモデルのvector型の配列</param>
	void update(const InputState& input);

	/// <summary>
	/// プレイヤー関連の描画
	/// </summary>
	void draw();

	VECTOR getRot();

	/// <summary>
	/// 外部からのポジションを受け取る
	/// </summary>
	/// <param name="pos">ポジション情報</param>
	void setPos(VECTOR pos);

	/// <summary>
	/// 外部からのジャンプ情報を受け取る
	/// </summary>
	/// <param name="isJump">ジャンプフラグ</param>
	/// <param name="jumpVec">ジャンプベクトル</param>
	void setJumpInfo(bool isJump, float jumpVec);

	void setClim(bool isClim) { isClim_ = isClim; }

	void setSaveData(VECTOR pos, int num, bool isContinue);
	int getDeathNum() {return deathCount_;	}

	PlayerStatus getStatus() { return status_; }

private:
	//通常更新
	void idleUpdate(const InputState& input);

	//待機処理
	void changeAnimIdle();

	//移動処理
	void movingUpdate(const InputState& input);

	//回転処理
	void rotationUpdate();

	//オブジェクトを登る
	void climUpdate(const InputState& input);

	//ジャンプ処理
	void jumpUpdate(const InputState& input);

	//走りジャンプ処理
	void runningJumpUpdate(const InputState& input);

	//死亡処理
	void deathUpdate(const InputState& input);

	//死体の後処理
	void deathPersonPostProsessing();

	//死人生成
	void deadPersonGenerater();

	//座る
	void sitUpdate(const InputState& input);

	//座っている途中
	void idleToSitup(const InputState& input);

	//立ち上がる
	void standUpdate(const InputState& input);

	//持ち運ぶ
	void carryObject();

private:
	/// <summary>
	/// プレイヤーの移動速度を設定する
	/// </summary>
	/// <returns>float型の移動速度</returns>
	float playerSpeed(bool pressedShift);

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
	bool isContinue_ = false;

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//中間ポイント

	VECTOR temp_ = { 0.0f,0.0f,0.0f };							//プレイヤーの移動ベクトル

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//死体のポジション

	std::shared_ptr<Model> PModel_;								//モデルクラスのポインタ
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ

	std::unordered_map<AnimType, int> animType_;				


	void(Player::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ
};

