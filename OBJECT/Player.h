#pragma once
#include "CharacterBase.h"
#include "util/PlayerData.h"
#include "util/ObjectData.h"
#include <DxLib.h>
#include <list>
#include <unordered_map>
#include <memory>

class InputState;
class Model;
class ObjectManager;
class ObjectBase;
class ManualCrank;

struct JumpInfo {
	float jumpVec = 0.0f;					//ジャンプベクトル
	bool isJump = false;					//ジャンプ中かどうか
};

struct Situation {
	bool isMoving = false;					//移動中か
	bool isSitting = false;					//座っているか
	bool isClim = false;					//登れるか
	bool isInTransit = false;				//運送中か
	bool isCanBeCarried = false;			//持ち運ぶことが出来るか
	bool isGimmickCanBeOperated = false;	//ギミックを操作中か
};

struct PlayerStatus {
	VECTOR pos;
	VECTOR rot;
	VECTOR moveVec;
	float height;
	int animNo;
	bool isAnimLoop;
	JumpInfo jump;
	Situation situation;
};

class Player
{
public:

	Player(const char* const filename);
	virtual ~Player();

	void Init(LoadObjectInfo info);

	/// <summary>
	/// プレイヤーの更新を行う
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	/// <param name="models">衝突判定を行うモデルのvector型の配列</param>
	void Update(const InputState& input,std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// プレイヤー関連の描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 死んだ回数を取得する
	/// </summary>
	int GetDeathCount() { return deathCount_; }

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

	void SetMoveVec(VECTOR vector) { status_.moveVec = vector; }

	/// <summary>
	/// 登れるかの情報を受け取る
	/// </summary>
	/// <returns>登れるかのフラグ</returns>
	void SetClim(bool isClim) { status_.situation.isClim = isClim; }

	/// <summary>
	/// 持ち運ぶ事が出来るフラグと持ち運ぶモデルのポインタを受け取る
	/// </summary>
	void SetCarryInfo(bool isCarry, std::shared_ptr<ObjectBase> model);

	/// <summary>
	/// プレイヤーのステータスを取得する
	/// </summary>
	PlayerStatus GetStatus() { return status_; }

	/// <summary>
	/// 持ち運ぶ死体のポインターを取得する
	/// </summary>
	std::shared_ptr<ObjectBase> GetDeadPersonModelPointer() {return deadPersonModelPointer_;}
	
	/// <summary>
	/// ManualCrankのポインタを取得する
	/// </summary>
	/// <param name="crank"></param>
	void SetGimmickModelPointer(std::shared_ptr<ManualCrank> crank);

	std::shared_ptr<Model> GetModelPointer() { return model_; }

	void BulletHitMe(VECTOR moveVec);

	void SetRoundShadowHeight(float height) { roundShadowHeight_ = height; }

private:
	//通常更新
	void NormalUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//待機処理
	void ChangeAnimIdle();

	//移動処理
	void MovingUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//移動
	float Move(const InputState& input);

	//回転処理
	void RotationUpdate();

	//ジャンプ処理
	void JumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//死亡処理
	void DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//死体の後処理
	void DeathPersonPostProsessing(std::shared_ptr<ObjectManager> objManager);

	//死人生成
	void DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager);

	//座る
	void SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//座っている途中
	void IdleToSitup(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//持ち運ぶ
	void CarryObjectUpdate();

	//荷物を下ろす
	void DropOffObjectUpdate();

	//クランクを回す
	void GoCrankRotationPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	void CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// クランクを回転させるアップデート
	/// </summary>
	void CrankRotatinUpdate(float rotZ);

	/// <summary>
	/// 投擲物との衝突アップデート
	/// </summary>
	void BulletHitMeUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

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
	/// <returns>ポジション</returns>
	VECTOR CenterFramPosition(const char* const LeftFramename, const char* const RightFramename);

	/// <summary>
	/// 指定したフレームの中心座標を算出する
	/// </summary>
	/// <returns>ポジション</returns>
	VECTOR FramPosition(const char* const framename);

	/// <summary>
	/// アニメーションの変更を行う
	/// </summary>
	/// <param name="type">アニメーションのタイプ</param>
	void ChangeAnimNo(PlayerAnimType type,bool isAnimLoop,int changeTime);

	/// <summary>
	/// 度数法から弧度法に変換して返す
	/// </summary>
	/// <param name="rot">角度</param>
	/// /// <returns>ポジション</returns>
	VECTOR DegreesToRadians(VECTOR rot);

	/// <summary>
	/// 落ち影の描画
	/// </summary>
	void DrawPolygon3D();

	/// <summary>
	/// 落ち影の頂点のポジションを取得する
	/// </summary>
	/// <param name="angle">角度</param>
	/// <returns>ポジション</returns>
	VECTOR VertexPosition(float angle);

private:

	int deathCount_ = 0;					//死んだ回数を記録する

	float totalAnimFrame_ = 0;
	float runJumpGravity = 0.0f;

	float targetAngle_ = 0.0f;				//回転
	float differenceAngle_ = 0.0f;			//目標の角度と現在の角度の差
	float angle_ = 0.0f;
	float roundShadowHeight_ = 0.0f;

	bool debugCreativeMode = false;

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//中間ポイント
	VECTOR scale_ = {0.0f,0.0f, 0.0f};							//拡縮率

	std::shared_ptr<Model> model_;								//モデルクラスのポインタ
	std::shared_ptr<ManualCrank> crank_;						//クランククラスのポインタ
	std::shared_ptr<ObjectBase> deadPersonModelPointer_;		//持ち運ぶ死体のモデルポインタ

	void(Player::* updateFunc_)(const InputState& input, std::shared_ptr<ObjectManager> objManager);		//メンバ関数ポインタ
	void(Player::* carryUpdateFunc_)();		//メンバ関数ポインタ
};

