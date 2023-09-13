#pragma once
#include "DxLib.h"
#include <memory>
#include <vector>
#include <deque>

class Player;
class Model;

constexpr int max_hit_coll = 2048;

class CheckCollisionModel
{
public:

	CheckCollisionModel();
	virtual ~CheckCollisionModel();

	void checkCollisionPersonalArea(Player& player,VECTOR moveVec, std::vector<std::shared_ptr<Model>> models);
	void checkCollisionWall(VECTOR moveVec, float playerHeight);
	void checkCollisionFloor(Player& player, VECTOR moveVec,bool jumpFlag,float playerHeight);
	void checkCollision(Player& player, VECTOR moveVec, std::vector<std::shared_ptr<Model>> model,float playerHeight,bool isJump,float jumpVec);

private:

	VECTOR oldPos;
	VECTOR nowPos;
	int i, j, k;
	int kabeNum;
	int yukaNum;
	bool moveFlag = false;
	bool hitFlag = false;

	//モデルとの当たり判定用メソッド
	std::vector<MV1_COLL_RESULT_POLY_DIM> hitDim_;
	MV1_COLL_RESULT_POLY* kabe[max_hit_coll];
	MV1_COLL_RESULT_POLY* yuka[max_hit_coll];
	MV1_COLL_RESULT_POLY* poly;
	HITRESULT_LINE lineRes;
};

