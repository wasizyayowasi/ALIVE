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

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="info">配置データ</param>
	Player(LoadObjectInfo info);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// プレイヤーの更新を行う
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	/// <param name="models">衝突判定を行うモデルのvector型の配列</param>
	void Update(std::shared_ptr<ObjectManager> objManager);

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

	/// <summary>
	/// ベクトルを設定する
	/// </summary>
	/// <param name="vector">ベクトル</param>
	void SetMoveVec(VECTOR vector) { status_.moveVec = vector; }

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
	void SetCrankPointer(std::shared_ptr<ManualCrank> crank);

	/// <summary>
	/// レバーのポインタを設定する
	/// </summary>
	void SetLeverPointer(std::shared_ptr<Lever> lever);

	/// <summary>
	/// モデルポインターを取得する
	/// </summary>
	/// <param name="height"></param>
	/// <param name="materialType"></param>
	std::shared_ptr<Model> GetModelPointer() { return model_; }

	/// <summary>
	/// 弾に当たったらノックバックを追加する
	/// </summary>
	void BulletHitMe(VECTOR moveVec);

	/// <summary>
	/// 地面に描画する影の高さを設定する
	/// </summary>
	/// <param name="height">高さ</param>
	/// <param name="materialType">今踏んでいるオブジェクトが何出てきているか</param>
	void SetRoundShadowHeightAndMaterial(float height, Material materialType);

	/// <summary>
	/// 影を描画する高さを取得する
	/// </summary>
	/// <returns>影を描画する高さ</returns>
	float GetRoundShadowHeight() { return roundShadowHeight_; }

private:
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void NormalUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// アニメーションをidleに変更する
	/// </summary>
	void ChangeAnimIdle();

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

	void tempRotationUpdate();

	/// <summary>
	/// 走りジャンプではないときのジャンプ
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	void JumpUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// プレイヤーの死体に与える情報を作る関数
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	void DeathUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// 死体の後処理
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void CorpsePostProsessing(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// プレイヤーの死体をvector配列で管理する関数
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void CorpseGenerater(std::shared_ptr<ObjectManager> objManager);

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
	void GoCrankRotationPosition(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// クランクの更新
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void CrankUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// クランクを回転させるアップデート
	/// </summary>
	/// <param name="rotZ">Zの回転率</param>
	void CrankRotationUpdate(float rotZ);

	/// <summary>
	/// 投擲物との衝突アップデート
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param> 
	void BulletHitMeUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// レバーを倒すポジションへ行く
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void GoLeverPullPosition(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// レバーの更新
	/// </summary>
	/// <param name="objManager">objManagerのポインタ</param>
	void LeverUpdate(std::shared_ptr<ObjectManager> objManager);

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
	/// 足ふみの音
	/// </summary>
	void FootStepSound();

	/// <summary>
	/// アニメーションの変更を行う
	/// </summary>
	/// <param name="type">アニメーションのタイプ</param>
	void ChangeAnimNo(PlayerAnimType type,bool isAnimLoop,int changeTime);

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

	float targetAngle_ = 0.0f;				//回転
	float differenceAngle_ = 0.0f;			//目標の角度と現在の角度の差
	float angle_ = 0.0f;
	float roundShadowHeight_ = 0.0f;

	bool debugCreativeMode = false;

	VECTOR tentativeRot_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//中間ポイント
	VECTOR scale_ = {0.0f,0.0f, 0.0f};							//拡縮率

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	Material materialSteppedOn_ = Material::Stone;

	std::shared_ptr<Model> model_;								//モデルクラスのポインタ
	std::shared_ptr<ManualCrank> crank_;						//クランククラスのポインタ
	std::shared_ptr<Lever> lever_;								//クランククラスのポインタ
	std::shared_ptr<ObjectBase> deadPersonModelPointer_;		//持ち運ぶ死体のモデルポインタ

	void(Player::* updateFunc_)(std::shared_ptr<ObjectManager> objManager);		//メンバ関数ポインタ
	void(Player::* carryUpdateFunc_)();		//メンバ関数ポインタ
};

