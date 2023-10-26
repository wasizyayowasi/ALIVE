#include "Aster.h"
#include "../util/ObjectManager.h"
#include <algorithm>

namespace {
	//二次元配列の最大X
	constexpr int max_X = 39 ;
	//二次元配列の最大Z
	constexpr int max_Y = 6;

	//二次元配列の最大要素数
	constexpr int max_Index = max_X * max_Y;
}

Aster::Aster()
{
	for (int i = 0; i < 234;i++) {
		masu_[i].centerPos = VGet((i % max_X) * 100 - 850, 0, (i / max_X) * 100 - 250);
		masu_[i].masuMode = MasuMode::normalMode;
		masu_[i].x = i % max_X;
		masu_[i].z = i / max_X;
	}

}

Aster::~Aster()
{
}

void Aster::Update()
{
	
}

void Aster::Draw()
{
	for (float x = -900.0f; x < 3000.0f; x += 100.0f) {
		DrawLine3D(VGet(x, 20, 300), VGet(x, 20, -300), 0xff0000);
	}
	for (float z = -300.0f; z < 300.0f; z += 100.0f) {
		DrawLine3D(VGet(-900, 20, z), VGet(3000, 20, z), 0xff0000);
	}

	for (int i = 0; i < max_Index; i++) {
		DrawSphere3D(masu_[i].centerPos, 16, 32, 0x00ff00, 0x00ff00, true);
		VECTOR pos = ConvWorldPosToScreenPos(masu_[i].centerPos);
		DrawFormatString(pos.x, pos.y, 0x448844, "%d", i);
	}

	DrawFormatString(0, 16, 0x448844, "敵がいるインデックス%d", enemyIndex_);
	DrawFormatString(0, 32, 0x448844, "プレイヤーがいるインデックス%d", playerIndex_);
	DrawFormatString(0, 48, 0x448844, "目的のインデックス%d", destination_.index);


	int y = 112;
	for (auto& result : score_) {
		DrawFormatString(0, y, 0x448844, "%d , score:%d , estination:%d , moveCost:%d",result.first, result.second.score, result.second.estimationCost, result.second.moveCost);
		y += 16;
	}

}

// ポジション情報を元に配列の何番目に存在するか取得する
VECTOR Aster::LocationInformation(VECTOR playerPos, VECTOR enemyPos)
{
	float min = 1000;

	for (int i = 0; i < max_Index; i++) {
		VECTOR distance = VSub(masu_[i].centerPos, enemyPos);
		float size = VSize(distance);
		if (min > size) {
			min = size;
			enemyIndex_ = i;
		}
	}

	min = 1000;
	for (int i = 0; i < max_Index; i++) {
		VECTOR distance = VSub(masu_[i].centerPos, playerPos);
		float size = VSize(distance);
		if (min > size) {
			min = size;
			playerIndex_ = i;
		}
	}

	return SearchAroundSquares();

}

/// 周囲の升が存在するか探す
VECTOR Aster::SearchAroundSquares()
{
	int leftEnd = enemyIndex_ % max_X;
	int rightEnd = enemyIndex_ % max_X;
	int bottom = enemyIndex_ / max_X;
	int top = enemyIndex_ / max_X;

	bool isCheckLeft = false;
	bool isCheckBottom = false;
	bool isCheckRight = false;
	bool isCheckTop = false;

	if (leftEnd == 0) {
		isCheckLeft = true;
	}
	if (bottom == 0) {
		isCheckBottom = true;
	}
	if (rightEnd == max_X - 1) {
		isCheckRight = true;
	}
	if (top == max_Y) {
		isCheckTop = true;
	}

	return SearchSurrroundingSquares(isCheckLeft, isCheckRight, isCheckTop, isCheckTop);

}

//特定の升のスコアを取得する
void Aster::ScoreCaluculation(Direction direction, int index)
{
	//推定コスト
	int estimationCostX = 0;
	int estimationCostZ = 0;

	//XとZどこに移動したか
	estimationCostX = masu_[playerIndex_].x - masu_[index].x;
	estimationCostZ = masu_[playerIndex_].z - masu_[index].z;

	//推定コストをdirectionの配列に入れる
	if (estimationCostX < 0 || estimationCostZ < 0) {
		if (estimationCostX < 0) {
			estimationCostX = -(std::min)(estimationCostX, 0);
		}
		if (estimationCostZ < 0) {
			estimationCostZ = -(std::min)(estimationCostZ, 0);
		}
		
		score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}
	else {
		score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}

	//移動量をカウントする
	score_[direction].moveCost = moveCount_;
	//移動コストと推定コストの結果を足してスコアを出す
	score_[direction].score = score_[direction].moveCost + score_[direction].estimationCost;
	//インデックスを入れる
	score_[direction].index = index;

}

//周囲の升のスコアを取得する
VECTOR Aster::SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom)
{
	//周囲の升のインデックス
	int left			= enemyIndex_ - 1;
	int topLeft			= enemyIndex_ + max_X - 1;
	int top				= enemyIndex_ + max_X;
	int topRight		= enemyIndex_ + max_X + 1;
	int right			= enemyIndex_ + 1;
	int bottomRight		= enemyIndex_ - max_X + 1;
	int bottom			= enemyIndex_ - max_X;
	int bottomLeft		= enemyIndex_ - max_X - 1;

	if (!skipCheckLeft) {
		//左の升のスコアを取得する
		if (masu_[left].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::left, left);
		}
		//左上の升のスコアを取得する
		if (masu_[topLeft].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::topLeft, topLeft);
		}
		//左下の升のスコアを取得する
		if (masu_[bottomLeft].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottomLeft, bottomLeft);
		}
	}

	if (!skipCheckRight) {
		//右の升のスコアを取得する
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);

		}
		//右上の升のスコアを取得する
		if (masu_[topRight].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::topRight, topRight);
		}
		//右下の升のスコアを取得する
		if (masu_[bottomRight].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottomRight, bottomRight);
		}
	}

	if (!skipCheckTop) {
		//上の升のスコアを取得する
		if (masu_[top].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::top, top);
		}
	}

	if (!skipCheckBottom) {
		//下の升のスコアを取得する
		if (masu_[bottom].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottom, bottom);
		}
	}

	//初期化
	destination_.score = 100;
	destination_.index = 0;
	//一番スコアが低い結果を取得する
	for (auto& result : score_) {
		if (destination_.score > result.second.score) {
			destination_.score = result.second.score;
			destination_.index = result.second.index;
		}
	}

	//score_.clear();

	return masu_[destination_.index].centerPos;

}