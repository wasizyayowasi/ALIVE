#pragma once
#include "PlayerData.h"
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class ObjectManager;
class ObjectBase;
class ManualCrank;
class Lever;

class Player
{
private:
	//計　6byte
	struct JumpInfo {
		float jumpVec = 0.0f;					//ジャンプベクトル		//4byte
		bool isJump = false;					//ジャンプ中かどうか	//2byte
	};

	//計　10byte
	struct Situation {
		bool isMoving = false;					//移動中か					//2byte			
		bool isClim = false;					//登れるか					//2byte
		bool isInTransit = false;				//運送中か					//2byte
		bool isCanBeCarried = false;			//持ち運ぶことが出来るか	//2byte
		bool isGimmickCanBeOperated = false;	//ギミックを操作中か		//2byte
	};

	//計　62byte
	struct PlayerStatus {
		int animNo;								//アニメーション番号				//4byte
		float height;							//高さ								//4byte
		VECTOR pos;								//ポジション						//12byte
		VECTOR rot;								//回転								//12byte
		VECTOR moveVec;							//移動ベクトル						//12byte
		JumpInfo jump;							//ジャンプ							//6byte
		Situation situation;					//シチュエーション					//10byte
		bool isAnimLoop;						//アニメーションのループが必要か	//2byte
	};

	//回転のステート
	enum class RotationState
	{
		Up,
		UpperRight,
		Right,
		LowerRight,
		Down,
		LowerLeft,
		Left,
		UpperLeft,

		Max,
	};

	//回転のデータ
	struct RotationData
	{
		float targetAngle_ = 0.0f;			//目標の角度
		RotationState rotState_ = {};		//回転の状態
		bool up			= false;			//上に行けるか
		bool upperRight	= false;			//右上に行けるか
		bool right		= false;			//右に行けるか
		bool lowerRight	= false;			//右下に行けるか
		bool down		= false;			//下に行けるか
		bool lowerLeft	= false;			//左下に行けるか
		bool left		= false;			//左に行けるか
		bool upperLeft	= false;			//左上に行けるか
	};

	//回転のデータ配列
	RotationData rotData_[static_cast<int>(RotationState::Max)]
	{
		//Up
		0.0f,						//目標の角度
		RotationState::Up,			//回転の状態
		true,						//上に行けるか
		true,						//右上に行けるか
		true,						//右に行けるか
		true,						//右下に行けるか
		false,						//下に行けるか
		false,						//左下に行けるか
		false,						//左に行けるか
		false,						//左上に行けるか

		//UpperRight
		45.0f,						//目標の角度
		RotationState::UpperRight,	//回転の状態
		true,						//上に行けるか
		true,						//右上に行けるか
		true,						//右に行けるか
		true,						//右下に行けるか
		true,						//下に行けるか
		false,						//左下に行けるか
		false,						//左に行けるか
		false,						//左上に行けるか

		//Right
		90.0f,						//目標の角度
		RotationState::Right,		//回転の状態
		true,						//上に行けるか
		true,						//右上に行けるか
		true,						//右に行けるか
		true,						//右下に行けるか
		true,						//下に行けるか
		true,						//左下に行けるか
		false,						//左に行けるか
		false,						//左上に行けるか

		//LowerRight
		135.0f,						//目標の角度
		RotationState::LowerRight,	//回転の状態
		true,						//上に行けるか
		true,						//右上に行けるか
		true,						//右に行けるか
		true,						//右下に行けるか
		true,						//下に行けるか
		true,						//左下に行けるか
		true,						//左に行けるか
		false,						//左上に行けるか

		//Down
		180.0f,						//目標の角度
		RotationState::Down,		//回転の状態
		false,						//上に行けるか
		true,						//右上に行けるか
		true,						//右に行けるか
		true,						//右下に行けるか
		true,						//下に行けるか
		true,						//左下に行けるか
		true,						//左に行けるか
		true,						//左上に行けるか

		//LowerLeft
		225.0f,						//目標の角度
		RotationState::LowerLeft,	//回転の状態
		true,						//上に行けるか
		false,						//右上に行けるか
		true,						//右に行けるか
		true,						//右下に行けるか
		true,						//下に行けるか
		true,						//左下に行けるか
		true,						//左に行けるか
		true,						//左上に行けるか

		//Left
		270.0f,						//目標の角度
		RotationState::Left,		//回転の状態
		true,						//上に行けるか
		true,						//右上に行けるか
		false,						//右に行けるか
		true,						//右下に行けるか
		false,						//下に行けるか
		true,						//左下に行けるか
		true,						//左に行けるか
		true,						//左上に行けるか

		//UpperLeft
		315.0f,						//目標の角度
		RotationState::UpperLeft,	//回転の状態
		true,						//上に行けるか
		true,						//右上に行けるか
		true,						//右に行けるか
		false,						//右下に行けるか
		true,						//下に行けるか
		true,						//左下に行けるか
		true,						//左に行けるか
		true,						//左上に行けるか
	};
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="info">配置データ</param>
	Player(const LoadObjectInfo& info);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// プレイヤーの更新を行う
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	/// <param name="models">衝突判定を行うモデルのvector型の配列</param>
	void Update(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// プレイヤー関連の描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 現在の角度から近い角度に回る
	/// </summary>
	/// <param name="differenceAngle">角度の違い</param>
	/// <param name="targetAngle">目標の角度</param>
	/// <param name="currentAngle">現在の角度</param>
	void RotateAtACloseAngle(float& differenceAngle, float targetAngle, float currentAngle);

	/// <summary>
	/// 弾に当たったらノックバックを追加する
	/// </summary>
	void BulletHitMe(const VECTOR& moveVec);

	/// <summary>
	/// どんな回転状態か取得する
	/// </summary>
	/// <returns>回転状況</returns>
	RotationState WhatRotationState();

	/// <summary>
	/// 目標の角度に回転することが出来るか
	/// </summary>
	/// <param name="rotZ">現在の角度</param>
	/// <returns>true:出来る  false:出来ない</returns>
	bool CanRotation(float rotZ);

	///////Getter///////

	/// <summary>
	/// 死んだ回数を取得する
	/// </summary>
	int GetDeathCount()const { return deathCount_; }

	/// <summary>
	/// プレイヤーのステータスを取得する
	/// </summary>
	const PlayerStatus& GetStatus() const { return status_; }

	/// <summary>
	/// 持ち運ぶ死体のポインターを取得する
	/// </summary>
	const std::shared_ptr<ObjectBase>& GetDeadPersonModelPointer() const { return corpseModelPointer_; }

	/// <summary>
	/// モデルポインターを取得する
	/// </summary>
	/// <param name="height"></param>
	/// <param name="materialType"></param>
	const std::shared_ptr<Model>& GetModelPointer() const { return model_; }

	/// <summary>
	/// 影を描画する高さを取得する
	/// </summary>
	/// <returns>影を描画する高さ</returns>
	float GetRoundShadowHeight() const { return roundShadowHeight_; }


	///////Setter///////

	/// <summary>
	/// 外部からのポジションを受け取る
	/// </summary>
	/// <param name="pos">ポジション情報</param>
	void SetPos(const VECTOR& pos);

	/// <summary>
	/// 外部からのジャンプ情報を受け取る
	/// </summary>
	/// <param name="isJump">ジャンプフラグ</param>
	/// <param name="jumpVec">ジャンプベクトル</param>
	void SetJumpInfo(const bool isJump, const  float jumpVec);

	/// <summary>
	/// ベクトルを設定する
	/// </summary>
	/// <param name="vector">ベクトル</param>
	void SetMoveVec(const VECTOR& vector) { status_.moveVec = vector; }

	/// <summary>
	/// 持ち運ぶ事が出来るフラグと持ち運ぶモデルのポインタを受け取る
	/// </summary>
	void SetCarryInfo(const bool isCarry,const std::shared_ptr<ObjectBase>& model);

	/// <summary>
	/// ManualCrankのポインタを設定する
	/// </summary>
	/// <param name="crank"></param>
	void SetCrankPointer(const std::shared_ptr<ManualCrank>& crank);

	/// <summary>
	/// レバーのポインタを設定する
	/// </summary>
	void SetLeverPointer(const std::shared_ptr<Lever>& lever);

	/// <summary>
	/// 地面に描画する影の高さと踏んでいるオブジェクトの素材を設定する
	/// </summary>
	/// <param name="height">高さ</param>
	/// <param name="materialType">今踏んでいるオブジェクトが何出てきているか</param>
	void SetRoundShadowHeightAndMaterial(const float height, const  Material materialType);

private:

	/////////////プレイヤーの挙動に関係する更新/////////////

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void NormalUpdate(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// 移動の更新
	/// </summary>
	void MovingUpdate();

	/// <summary>
	/// 移動
	/// </summary>
	float Move();

	/// <summary>
	/// 回転処理
	/// </summary>
	void RotationUpdate();

	/// <summary>
	/// ジャンプ
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	void JumpUpdate(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// プレイヤーの死体に与える情報を作る関数
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	void DeathUpdate(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// 死体の後処理
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void CorpsePostProsessing(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// プレイヤーの死体をvector配列で管理する関数
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void CorpseGenerater(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// 荷物を運ぶ
	/// </summary>
	void CarryObject();

	/// <summary>
	/// 荷物をおろす
	/// </summary>
	void DropOffObject();

	/// <summary>
	/// クランクを回すポジションまで行く
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void GoCrankRotationPosition(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// クランクを回転させるアップデート
	/// </summary>
	/// <param name="rotZ">Zの回転率</param>
	void CrankRotationUpdate(float rotZ);

	/// <summary>
	/// クランクの更新
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void CrankUpdate(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// レバーを倒すポジションへ行く
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void GoLeverPullPosition(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// レバーの更新
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void LeverUpdate(const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// 投擲物との衝突アップデート
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param> 
	void BulletHitMeUpdate(const std::shared_ptr<ObjectManager>& objManager);

private:

	/////////////プレイヤーのステータス等に関係する更新/////////////

	/// <summary>
	/// アニメーションをidleに変更する
	/// </summary>
	void ChangeAnimIdle();

	/// <summary>
	/// アニメーションの変更を行う
	/// </summary>
	/// <param name="type">アニメーションのタイプ</param>
	void ChangeAnimNo(const PlayerAnimType type, const  bool isAnimLoop, const  int changeTime);

	/// <summary>
	/// プレイヤーの移動速度を設定する
	/// </summary>
	/// <returns>float型の移動速度</returns>
	float PlayerSpeed(const bool pressedShift);

	/// <summary>
	/// 足ふみの音
	/// </summary>
	void FootStepSound();

	/// <summary>
	/// 落ち影の描画
	/// </summary>
	void DrawPolygon3D();

	/// <summary>
	/// 落ち影の頂点のポジションを取得する
	/// </summary>
	/// <param name="angle">角度</param>
	/// <returns>ポジション</returns>
	VECTOR VertexPosition(const float angle);

private:

	int deathCount_ = 0;										//死んだ回数を記録する
	int crankLaps_ = 0;											//クランクの周回数

	float targetAngle_ = 0.0f;									//回転
	float differenceAngle_ = 0.0f;								//目標の角度と現在の角度の差
	float angle_ = 0.0f;
	float roundShadowHeight_ = 0.0f;
	float crankTargetAngle_ = 0.0f;								//クランクが目指す角度

	bool debugCreativeMode_ = false;							//デバッグ用変数

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//中間ポイント
	VECTOR scale_ = {0.0f,0.0f, 0.0f};							//拡縮率

	PlayerInfo playerInfo_ = {};								//プレイヤーの情報
	PlayerStatus status_ = {};									//プレイヤーのステータス

	Material materialSteppedOn_ = Material::Stone;				//足音用変数
	RotationState currentRotState_ = RotationState::Up;			//回転ステータス

	std::shared_ptr<Model> model_;								//モデルクラスのポインタ
	std::shared_ptr<ManualCrank> crank_;						//クランククラスのポインタ
	std::shared_ptr<Lever> lever_;								//クランククラスのポインタ
	std::shared_ptr<ObjectBase> corpseModelPointer_;			//持ち運ぶ死体のモデルポインタ

	void(Player::* updateFunc_)(const std::shared_ptr<ObjectManager>& objManager);		//メンバ関数ポインタ
	void(Player::* carryUpdateFunc_)();		//メンバ関数ポインタ
};

