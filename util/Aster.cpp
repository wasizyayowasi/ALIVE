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
		masu_[i].centerPos = VGet((i % max_X) * 100.0f - 850.0f, 0.0f, (i / max_X) * 100.0f - 250.0f);
		masu_[i].masuMode = MasuMode::normalMode;
		masu_[i].x = i % max_X;
		masu_[i].z = i / max_X;
	}

	masu_[9].masuMode = MasuMode::blockadeMode;
	masu_[48].masuMode = MasuMode::blockadeMode;
	masu_[87].masuMode = MasuMode::blockadeMode;
	masu_[126].masuMode = MasuMode::blockadeMode;
//	masu_[165].masuMode = MasuMode::blockadeMode;
	masu_[204].masuMode = MasuMode::blockadeMode;

//	masu_[46].masuMode = MasuMode::blockadeMode;
//	masu_[47].masuMode = MasuMode::blockadeMode;
//	masu_[48].masuMode = MasuMode::blockadeMode;
//	masu_[85].masuMode = MasuMode::blockadeMode;
//	masu_[86].masuMode = MasuMode::blockadeMode;
//	masu_[87].masuMode = MasuMode::blockadeMode;
//	masu_[124].masuMode = MasuMode::blockadeMode;
//	masu_[125].masuMode = MasuMode::blockadeMode;
//	masu_[126].masuMode = MasuMode::blockadeMode;

}

Aster::~Aster()
{
}

void Aster::Update()
{
	
}

void Aster::Draw()
{
	//デバッグ用描画
	{
		for (float x = -900.0f; x < 3000.0f; x += 100.0f) {
			DrawLine3D(VGet(x, 20, 300), VGet(x, 20, -300), 0xff0000);
		}
		for (float z = -300.0f; z < 300.0f; z += 100.0f) {
			DrawLine3D(VGet(-900, 20, z), VGet(3000, 20, z), 0xff0000);
		}

		for (int i = 0; i < max_Index; i++) {
			if (masu_[i].masuMode == MasuMode::normalMode) {
				DrawSphere3D(masu_[i].centerPos, 16, 32, 0x00ff00, 0x00ff00, true);
			}
			else if(masu_[i].masuMode == MasuMode::blockadeMode) {
				DrawSphere3D(masu_[i].centerPos, 16, 32, 0xff0000, 0xff0000, true);
			}
			else {
				DrawSphere3D(masu_[i].centerPos, 16, 32, 0xffff00, 0xffff00, true);
			}
			
			VECTOR pos = ConvWorldPosToScreenPos(masu_[i].centerPos);
			DrawFormatString(pos.x, pos.y, 0x448844, "%d", i);
		}

		DrawFormatString(0, 16, 0x448844, "敵がいるインデックス%d", enemyIndex_);
		DrawFormatString(0, 32, 0x448844, "プレイヤーがいるインデックス%d", playerIndex_);
		DrawFormatString(0, 48, 0x448844, "目的のインデックス%d", destination_.index);

		//int y = 128;
		for (auto& result : tempscore_) {
			VECTOR pos = ConvWorldPosToScreenPos(masu_[result.second.index].centerPos);
			DrawFormatString(pos.x, pos.y + 10, 0xff0000, "S:%d", result.second.score);
			//y += 16;
		}
	}
}

// ポジション情報を元に配列の何番目に存在するか取得する
VECTOR Aster::LocationInformation(VECTOR playerPos, VECTOR enemyPos)
{
	

	int oldEnemyIndex = enemyIndex_;

	//敵のインデックスを取得
	enemyIndex_ = SearchCurrentIndex(enemyPos);

	//一個前にいたインデックス座標の升のモードを通過済みにする
	if (oldEnemyIndex != enemyIndex_) {
		if (masu_[oldEnemyIndex].masuMode != MasuMode::blockadeMode) {
			//一個前の升を通過済みにする
			masu_[oldEnemyIndex].masuMode = MasuMode::passingMode;
			//通過した升のインデックスを保存する
			preteriteIndex_.push_back(oldEnemyIndex);
			//保存した升のインデックスが2個以上になったら
			//一番古い升のModeをnormalにし、一番古いデータを削除する
			if (preteriteIndex_.size() > 10) {
				masu_[preteriteIndex_.front()].masuMode = MasuMode::normalMode;
				preteriteIndex_.remove(preteriteIndex_.front());
			}
		}
	}

	//プレイヤーのインデックスを取得
	playerIndex_ = SearchCurrentIndex(playerPos);

	//プレイヤーと敵のインデックスが同じだった場合
	//そのマスの中心座標を返す
	if (playerIndex_ == enemyIndex_) {
		for (auto& mode : masu_) {
			if (mode.second.masuMode != MasuMode::blockadeMode) {
				mode.second.masuMode = MasuMode::normalMode;
			}
		}
		return masu_[playerIndex_].centerPos;
	}

	return SearchAroundSquares();

}

/// 周囲の升が存在するか探す
VECTOR Aster::SearchAroundSquares()
{
	//両端、上下の端のマスを取得
	int leftEnd = enemyIndex_ % max_X;
	int rightEnd = enemyIndex_ % max_X;
	int bottom = enemyIndex_ / max_X;
	int top = enemyIndex_ / max_X;

	//マスがないところは見ないようにする
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

bool Aster::StraightLineDistanceSearch(VECTOR playerPos, VECTOR enemyPos)
{
	VECTOR distance = VSub(playerPos, enemyPos);

	float size = VSize(distance);
	int num = size / 100;
	bool isBlockade = false;

	//敵のインデックスを取得
	enemyIndex_ = SearchCurrentIndex(enemyPos);
	//プレイヤーのインデックスを取得
	playerIndex_ = SearchCurrentIndex(playerPos);

	for (int i = 0; i < num; i++) {
		distance = VSub(playerPos, enemyPos);
		VECTOR norm = VNorm(distance);
		VECTOR scale = VScale(norm, i * 100.0f);
		VECTOR pos = VAdd(enemyPos, scale);

		int x = pos.x / 100;
		int z = pos.z / 100;

		int temp = enemyIndex_ + (x * max_X + z);

		//masu_[temp].masuMode = MasuMode::blockadeMode;
		if (masu_[x + z * max_X].masuMode == MasuMode::blockadeMode) {
			isBlockade = true;
		}

	}

	return isBlockade;
}

int Aster::SearchCurrentIndex(VECTOR pos)
{

	int index = 0;
	float min = 1000.0f;
	float size = 0.0f;
	VECTOR distance = {};

	//座標から一番近いマスの中心を見つけindex番号を取得する
	for (int i = 0; i < max_Index; i++) {
		//中心から敵のポジションの距離をとる
		distance = VSub(masu_[i].centerPos, pos);
		size = VSize(distance);
		//過去最短に近い結果と比べる
		if (min > size) {
			//過去最短を更新
			min = size;
			//インデックスを取得
			index = i;
		}
	}

	//インデックスを取得
	return index;
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

	if (!skipCheckLeft) {
		//左の升のスコアを取得する
		if (masu_[left].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::left, left);
		}
		////左上の升のスコアを取得する
		//if (masu_[topLeft].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::topLeft, topLeft);
		//}
		////左下の升のスコアを取得する
		//if (masu_[bottomLeft].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::bottomLeft, bottomLeft);
		//}
	}

	if (!skipCheckRight) {
		//右の升のスコアを取得する
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);
		}
		////右上の升のスコアを取得する
		//if (masu_[topRight].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::topRight, topRight);
		//}
		////右下の升のスコアを取得する
		//if (masu_[bottomRight].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::bottomRight, bottomRight);
		//}
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

	//デバッグ用
	tempscore_ = score_;

	score_.clear();

	return masu_[destination_.index].centerPos;

}