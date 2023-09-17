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
	int hitWallNum;
	int hitFloorNum;
	bool moveFlag = false;
	bool hitFlag = false;

	//���f���Ƃ̓����蔻��p���\�b�h
	std::vector<MV1_COLL_RESULT_POLY_DIM> hitDim_;
	MV1_COLL_RESULT_POLY* wallHitDim_[max_hit_coll];
	MV1_COLL_RESULT_POLY* floorHitDim_[max_hit_coll];
	MV1_COLL_RESULT_POLY* hitPoly;
	HITRESULT_LINE hitLineResult;
};

