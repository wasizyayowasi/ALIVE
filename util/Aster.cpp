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

}

Aster::~Aster()
{
}

void Aster::Init()
{
	route_.clear();
	scoreTable_.clear();
	for (auto& masu : masu_) {
		if (masu.second.masuMode == MasuMode::doneMode) {
			masu.second.masuMode = MasuMode::normalMode;
		}
	}
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
		if (!route_.empty()) {
			DrawFormatString(0, 48, 0x448844, "目的のインデックス%d", route_.front());
		}

		//int y = 128;
		for (auto& result : debugScoreTable) {
			VECTOR pos = ConvWorldPosToScreenPos(masu_[result.second.destinationIndex].centerPos);
			DrawFormatString(pos.x, pos.y + 10, 0xff0000, "S:%d", result.second.score);
			//y += 16;
		}
	}
}

// ポジション情報を元に配列の何番目に存在するか取得する
void Aster::LocationInformation(VECTOR playerPos, VECTOR enemyPos)
{
	int oldEnemyIndex = enemyIndex_;
	int oldPlayerIndex = playerIndex_;

	//敵のインデックスを取得
	enemyIndex_ = SearchCurrentIndex(enemyPos);

	//プレイヤーのインデックスを取得
	playerIndex_ = SearchCurrentIndex(playerPos);

	//プレイヤーと敵のインデックスが同じだった場合
	//そのマスの中心座標を返す
	if (playerIndex_ == enemyIndex_) {
		for (auto& mode : masu_) {
			if (mode.second.masuMode != MasuMode::blockadeMode) {
				mode.second.masuMode = MasuMode::normalMode;
				moveCount_ = 0;
			}
		}
	}

	//プレイヤーのインデックス座標が前のフレームと比べたとき
	//違ったら、再度経路探索を行う
	if (oldPlayerIndex != playerIndex_ && masu_[playerIndex_].masuMode != MasuMode::blockadeMode) {
		Init();
		RouteSearch();
	}

}

/// 周囲の升が存在するか探す
void Aster::SearchAroundSquares()
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

	SearchSurrroundingSquares(isCheckLeft, isCheckRight, isCheckTop, isCheckBottom);

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
		
		//四方向の時
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;

		//八方向の時
		//score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}
	else {
		//四方向の時
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;

		//八方向の時
		//score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}

	//移動量をカウントする
	if (masu_[index].masuMode == MasuMode::normalMode) {
		scoreTable_[index].moveCost = moveCount_;
		scoreTable_[index].score = scoreTable_[index].moveCost + scoreTable_[index].estimationCost;
		scoreTable_[index].destinationIndex = index;
		scoreTable_[index].currentIndex = enemyIndex_;
		scoreTable_[index].dir = direction;
	}

	preteriteIndex_[moveCount_].push_back(index);

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

void Aster::RouteSearch()
{

	int currentIndex = enemyIndex_;

	while (enemyIndex_ != playerIndex_)
	{
		if (masu_[enemyIndex_].masuMode != MasuMode::blockadeMode) {
			masu_[enemyIndex_].masuMode = MasuMode::doneMode;
		}

		SearchAroundSquares();

		//次にスコアを出すマスが既にスコアが出してあった場合
		//何回目にスコア計算が行われたかを取得する
		for (auto& list : preteriteIndex_) {
			for (auto& index : list.second) {
				if (index == enemyIndex_) {
					moveCount_ = list.first;
				}
			}
		}
		moveCount_++;
	}

	int targetIndex = playerIndex_;

	//最短ルートをscoreTableから抽出する
	while (currentIndex != targetIndex)
	{
		targetIndex = scoreTable_[targetIndex].currentIndex;
		route_.push_front(targetIndex);
	}
	//最後にプレイヤーがいるインデックス座標を追加する
	route_.push_back(playerIndex_);

	//リセット
	scoreTable_.clear();
	debugScoreTable.clear();

}

VECTOR Aster::GetDestinationCoordinates(VECTOR playerPos,VECTOR enemyPos)
{
	//エネミーとプレイヤーのインデックス座標を取得
	int enemyIndex = SearchCurrentIndex(enemyPos);
	int playerIndex = SearchCurrentIndex(playerPos);

	if (masu_[playerIndex].masuMode == MasuMode::blockadeMode) {
		return masu_[enemyIndex].centerPos;
	}

	//プレイヤーとエネミーが同じインデックス座標に居る場合
	if (enemyIndex == playerIndex) {
		//最短ルートを削除する
		route_.clear();

		//最短ルートを探す際にマスのモードをdoneModeにしたため
		//doneModeからnormalModeに戻す
		for (auto& masu : masu_) {
			if (masu.second.masuMode == MasuMode::doneMode) {
				masu.second.masuMode = MasuMode::normalMode;
			}
		}

		//プレイヤーの居るインデックス座標の中心のポジションを返す
		return masu_[playerIndex].centerPos;
	}

	//目標インデックス座標の中心ポジション
	VECTOR targetPos = masu_[route_.front()].centerPos;

	//敵のインデックス座標と経路探索で得たインデックスが同じだった場合
	//同じインデックス座標を削除する
	if (enemyIndex == route_.front()) {
		route_.pop_front();
	}

	//目標ポジションを返す
	return targetPos;
}

bool Aster::temp(VECTOR pos)
{
	int pointIndex = SearchCurrentIndex(pos);

	if (masu_[pointIndex].masuMode == MasuMode::blockadeMode) {
		return true;
	}

	return false;
}

bool Aster::EndOfDirection()
{
	auto aiu = route_.empty();
	return route_.empty();
}

//周囲の升のスコアを取得する
void Aster::SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom)
{
	//周囲の升のインデックス
	int left			= enemyIndex_ - 1;
	int right			= enemyIndex_ + 1;
	int top				= enemyIndex_ + max_X;
	int bottom			= enemyIndex_ - max_X;

	int topLeft			= enemyIndex_ + max_X - 1;
	int topRight		= enemyIndex_ + max_X + 1;
	int bottomRight		= enemyIndex_ - max_X + 1;
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
	}

	if (!skipCheckRight) {
		//右の升のスコアを取得する
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);
		}
	}

	//初期化
	int score = 1000;

	//最短ルートを求める際に一番低いスコアのインデックスマスを次の探索マスにする
	for (auto& result : scoreTable_) {
		if (masu_[result.second.destinationIndex].masuMode == MasuMode::normalMode) {
			if (score > result.second.score) {
				score = result.second.score;
				enemyIndex_ = result.second.destinationIndex;
				count_++;
			}
		}
	}

	//デバッグ用
	debugScoreTable = scoreTable_;

}