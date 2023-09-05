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

	VECTOR getPos() { return playerPos_; }

private:

	int animNo_ = 0;						//現在のアニメーション番号
	int deathNum = 0;						//死亡回数

	float movingSpeed_ = 5.0f;				//移動速度
	float jumpVec_ = 0.0f;					//ジャンプベクトル

	bool jumpFlag_ = false;					//ジャンプしているかどうかのフラグ
	bool tempBool = false;

	VECTOR playerPos_ = { 0,16,0 };			//プレイヤーのポジション

	std::vector<DeadPlayer> deadPlayer_;	//死体を保存するため
	std::shared_ptr<Model> model_;

};

