#pragma once
#include "DxLib.h"
#include <memory>
#include <vector>

class Player;
class Model;

constexpr int max_hit_coll = 2048;

class CheckCollitionModel
{
public:

	CheckCollitionModel(std::shared_ptr<Player> player);
	virtual ~CheckCollitionModel();

	void checkCollitionPersonalArea(VECTOR moveVec, std::vector<std::shared_ptr<Model>> models);
	void checkCollitionWall(VECTOR moveVec, float playerHeight);
	void checkCollitionFloor(VECTOR moveVec,bool jumpFlag,float playerHeight);
	void checkCollition(VECTOR moveVec, std::vector<std::shared_ptr<Model>> model,float playerHeight,bool isJump,float jumpVec);

private:

	VECTOR oldPos;
	VECTOR nowPos;
	int i, j, k;
	int kabeNum;
	int yukaNum;
	bool moveFlag = false;
	bool hitFlag = false;

	//モデルとの当たり判定用メソッド
	MV1_COLL_RESULT_POLY_DIM HitDim[11];
	MV1_COLL_RESULT_POLY* kabe[max_hit_coll];
	MV1_COLL_RESULT_POLY* yuka[max_hit_coll];
	MV1_COLL_RESULT_POLY* poly;
	HITRESULT_LINE lineRes;

	std::shared_ptr<Player> player_;

};

