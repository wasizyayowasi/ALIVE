#pragma once
#include "DxLib.h"
#include "CharacterBase.h"
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

enum class AnimType {
	idle,			//待機状態
	run,			//走る
	stairs,			//階段を上る
	radder,			//上る
	jump,			//ジャンプ
	runningJump,	//走りジャンプ
	death,			//走りジャンプ
	walk,			//歩く
	idleToSitup,	//idle状態から座る
	situpToIdle,	//座っている状態からidle
	sit,			//座っている
	clim,			//登る
	stand,			//立ち上がる
	carry,			//運ぶ
	carryWalking,	//運びながら歩く
	max
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int anim_[static_cast<int>(AnimType::max)];
};

class Player:public CharacterBase
{
	friend CheckCollisionModel;
	friend GimmickBase;
public:

	Player(const char* const filename);
	Player(int handle);
	virtual ~Player();

	void init();

	void update(const InputState& input);
	void draw();

	VECTOR getPos() { return pos_; }
	VECTOR getRot();
	void setPos(VECTOR pos);
	JumpInfo getJumpInfo() { return jump_; }
	void setJumpInfo(bool isJump, float jumpVec);

	VECTOR getMoveVec() { return moveVec_; }

	void setSaveData(VECTOR pos, int num, bool isContinue);
	int getDeathNum() {return deathCount_;	}
	std::list<std::shared_ptr<Model>> getDeadPerson() { return deadPerson_; }
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

	//立ち上がる
	void standUpdate(const InputState& input);

	//持ち運ぶ
	void carryObject();

private:

	/// <summary>
	/// 死亡パターン別、衝突情報の設定
	/// </summary>
	void setCollitionInfoByDeathPattern();

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

	JumpInfo jump_;												//ジャンプ関連の構造体


	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//中間ポイント

	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };						//プレイヤーの移動ベクトル
	VECTOR temp_ = { 0.0f,0.0f,0.0f };							//プレイヤーの移動ベクトル

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//死体のポジション

	std::shared_ptr<Model> PModel_;								//モデルクラスのポインタ
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ

	std::list<std::shared_ptr<Model>> deadPerson_;				//死体を保存するため

	std::unordered_map<AnimType, int> animType_;				

	PlayerInfo playerInfo;

	void(Player::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ
};

