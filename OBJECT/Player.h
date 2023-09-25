#pragma once
#include "DxLib.h"
//#include "CharacterBase.h"
#include <vector>
#include <unordered_map>
#include <memory>

class InputState;
class Model;
class CheckCollisionModel;

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

class Player
{
	friend CheckCollisionModel;
public:

	Player();
	virtual ~Player();

	void update(const InputState& input, std::vector<std::shared_ptr<Model>> models);
	void draw();

	VECTOR getPos() { return pos_; }
	VECTOR getRot();
	void setPos(VECTOR pos);
	JumpInfo getJumpInfo() { return jump_; }
	void setJumpInfo(bool isJump, float jumpVec);

	void idleUpdate(const InputState& input);

private:
	//移動処理
	void movingUpdate(const InputState& input);
	//ジャンプ処理
	void jumpUpdate(const InputState& input);
	//走りジャンプ処理
	void runningJumpUpdate(const InputState& input);
	//死亡処理
	void death(const InputState& input);
	//待機処理
	void changeAnimIdle();
	//回転処理
	void rotationUpdate();
	//死人生成
	void deadPersonGenerater();
	//座る
	void sitUpdate(const InputState& input);
	//オブジェクトを登る
	void climUpdate(const InputState& input);
	//立ち上がる
	void standUpdate(const InputState& input);

private:

	int temp = 0;
	float tempGravity = 0.0f;

	int animNo_ = 0;						//現在のアニメーション番号
	

	float movingSpeed_ = 0.0f;				//移動速度
	float targetAngle_ = 0.0f;				//回転
	float differenceAngle_ = 0.0f;			//目標の角度と現在の角度の差
	float tempAngle_ = 0.0f;

	bool isMoving = false;					//移動中か
	bool isDead_ = false;					//死んでいるか
	bool isSitting_ = false;				//座っているか
	bool isAnimLoop_ = true;				//アニメーションをループさせるか
	bool isClim_ = false;

	JumpInfo jump_;							//ジャンプ関連の構造体


	VECTOR checkPoint_ = { 0.0f,0.0f, 0.0f };					//中間ポイント

	VECTOR pos_ = { 0.0f,0.0f,0.0f };							//プレイヤーのポジション
	VECTOR rot_ = { 0.0f,0.0f,0.0f };							//プレイヤーの回転
	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };						//プレイヤーの移動ベクトル
	VECTOR temp_ = { 0.0f,0.0f,0.0f };						//プレイヤーの移動ベクトル

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//死体のポジション

	std::shared_ptr<Model> PModel_;								//モデルクラスのポインタ
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ
	std::vector<std::shared_ptr<Model>> deadPlayer_;			//死体を保存するため

	std::unordered_map<AnimType, int> animType_;				

	PlayerInfo playerInfo;

	void(Player::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ
};

