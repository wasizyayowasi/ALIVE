#pragma once
#include "DxLib.h"
#include "CharacterBase.h"
#include "util/PlayerData.h"
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

struct PlayerStatus {
	JumpInfo jump;
	VECTOR pos;
	VECTOR rot;
	VECTOR moveVec;
	int animNo;
	float height;
	bool isAnimLoop;
	bool isTransit;
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
	/// 死んだ回数を取得する
	/// </summary>
	int getDeathCount() { return deathCount_; }

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

	/// <summary>
	/// 登れるかの情報を受け取る
	/// </summary>
	/// <returns>登れるかのフラグ</returns>
	void SetClim(bool isClim) { isClim_ = isClim; }

	/// <summary>
	/// 持ち運ぶ事が出来るフラグと持ち運ぶモデルのポインタを受け取る
	/// </summary>
	void SetCarryInfo(bool isCarry, std::shared_ptr<Model> model);

	void SetSaveData(VECTOR pos);

	/// <summary>
	/// プレイヤーのステータスを取得する
	/// </summary>
	PlayerStatus GetStatus() { return status_; }

	/// <summary>
	/// 持ち運ぶ死体のポインターを取得する
	/// </summary>
	std::shared_ptr<Model> GetDeadPersonModelPointer() {return deadPersonModelPointer_;}
		
private:
	//通常更新
	void IdleUpdate(const InputState& input);

	//待機処理
	void ChangeAnimIdle();

	//移動処理
	void MovingUpdate(const InputState& input);

	//移動
	float Move(const InputState& input);

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
	void CarryObjectUpdate();

	//荷物を下ろす
	void DropOffObjectUpdate();

private:
	/// <summary>
	/// プレイヤーの移動速度を設定する
	/// </summary>
	/// <returns>float型の移動速度</returns>
	float PlayerSpeed(bool pressedShift);

	/// <summary>
	/// ジャンプの設定を行う
	/// </summary>
	/// <param name="jumpPower">ジャンプ力</param>
	void PlayerJump(float jumpPower);

	/// <summary>
	/// 指定した2フレームの中心座標を算出する
	/// </summary>
	VECTOR FramPosition(const char* const LeftFramename, const char* const RightFramename);

	/// <summary>
	/// アニメーションの変更を行う
	/// </summary>
	/// <param name="type">アニメーションのタイプ</param>
	void ChangeAnimNo(AnimType type,bool isAnimLoop,int changeTime);

	/// <summary>
	/// 度数法から弧度法に変換して返す
	/// </summary>
	/// <param name="rot">角度</param>
	VECTOR DegreesToRadians(VECTOR rot);

private:

	int deathCount_ = 0;					//死んだ回数を記録する

	float temp = 0;
	float tempGravity = 0.0f;

	float targetAngle_ = 0.0f;				//回転
	float differenceAngle_ = 0.0f;			//目標の角度と現在の角度の差
	float tempAngle_ = 0.0f;

	bool isMoving_ = false;					//移動中か
	bool isSitting_ = false;				//座っているか
	bool isClim_ = false;
	bool isCanBeCarried_ = false;

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//中間ポイント

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//死体のポジション

	std::shared_ptr<Model> deadPersonModelPointer_;				//持ち運ぶ死体のモデルポインタ
	std::shared_ptr<Model> PModel_;								//モデルクラスのポインタ
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ

	std::unordered_map<AnimType, int> animType_;				

	void(Player::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ
	void(Player::* carryUpdateFunc_)();		//メンバ関数ポインタ
};

