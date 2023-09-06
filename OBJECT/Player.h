#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>

class InputState;
class Model;

struct DeadPlayer {
	bool isEnable;
	VECTOR deathPos;
};

class Player
{
public:

	Player();
	virtual ~Player();

	void update(const InputState& input);
	void draw();

	VECTOR getPos() { return pos_; }

private:
	//移動処理
	void moving(const InputState& input);
	//ジャンプ処理
	void jump(const InputState& input);
	//死亡処理
	void death(const InputState& input);
	//待機処理
	void idle();

private:

	int animNo_ = 0;						//現在のアニメーション番号
	int deathNum = 0;						//死亡回数

	float movingSpeed_ = 5.0f;				//移動速度
	float jumpVec_ = 0.0f;					//ジャンプベクトル
	float targetAngle_ = 0.0f;					//回転
	float angle_ = 0.0f;

	bool jumpFlag_ = false;					//ジャンプしているかどうかのフラグ
	bool isMoving = false;					//移動中か

	VECTOR pos_ = { 0,16,0 };			//プレイヤーのポジション
	VECTOR rot_ = { 0.0f,0.0f,0.0f };	//プレイヤーの回転

	std::vector<DeadPlayer> deadPlayer_;	//死体を保存するため
	std::shared_ptr<Model> model_;
};

