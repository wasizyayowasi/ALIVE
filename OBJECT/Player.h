#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>

class InputState;
class Model;
class CheckCollitionModel;

struct DeadPlayer {
	bool isEnable;
	VECTOR deathPos;
};

struct JumpInfo {
	float jumpVec;		//ジャンプベクトル
	bool isJump;		//ジャンプ中かどうか
};

class Player
{
public:

	Player();
	virtual ~Player();

	void update(const InputState& input,std::shared_ptr<Model> model);
	void draw();

	VECTOR getPos() { return pos_; }
	void setPos(VECTOR pos) { pos_ = pos; }
	JumpInfo getJumpInfo() { return jump_; }
	void setJumpInfo(bool isJump, float jumpVec);

private:
	//移動処理
	void moving(const InputState& input);
	//ジャンプ処理
	void jump(const InputState& input);
	//死亡処理
	void death(const InputState& input);
	//待機処理
	void idle();
	//回転処理
	void rotation();

private:

	int animNo_ = 0;						//現在のアニメーション番号
	int deathNum = 0;						//死亡回数

	float movingSpeed_ = 5.0f;				//移動速度
	float targetAngle_ = 0.0f;				//回転
	float differenceAngle_ = 0.0f;			//目標の角度と現在の角度の差
	float tempAngle_ = 0.0f;

	bool isMoving = false;					//移動中か

	JumpInfo jump_;							//ジャンプ関連の構造体

	VECTOR pos_ = { 0.0f,0.0f,0.0f };		//プレイヤーのポジション
	VECTOR rot_ = { 0.0f,0.0f,0.0f };		//プレイヤーの回転
	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };	//プレイヤーの移動ベクトル

	std::vector<DeadPlayer> deadPlayer_;	//死体を保存するため
	std::shared_ptr<Model> model_;
	std::shared_ptr<CheckCollitionModel> checkCollitionModel_;
};

