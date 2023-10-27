#pragma once
#include "DxLib.h"
#include <unordered_map>
#include <list>

enum class MasuMode {
	normalMode,			//通常モード
	passingMode,		//通過済みモード
	blockadeMode,		//封鎖モード
};

enum class Direction {
	left,				//左
	topLeft,			//左上
	top,				//上
	topRight,			//右上
	right,				//右
	bottomRight,		//右下
	bottom,				//下
	bottomLeft,			//左下
};

struct MasuState {
	VECTOR centerPos;	//中心座標
	MasuMode masuMode;	//升のモード
	int x;				//2次元配列のX
	int z;				//2次元配列のY
};

struct Score {
	int moveCost;		//移動コスト
	int estimationCost;	//推定コスト
	int score;			//プレイヤーを追跡するうえで得点を付ける
	int index;			//インデックス
};

struct DesinationState {
	int index;
	int score;
};

class Aster
{
public:
	Aster();
	virtual ~Aster();

	//更新
	void Update();
	//描画
	void Draw();

	/// <summary>
	/// ポジション情報を元に配列の何番目に存在するか取得する
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="pos">ポジション</param>
	VECTOR LocationInformation(VECTOR playerPos,VECTOR enemyPos);

	/// <summary>
	/// 周囲の升を検索する
	/// </summary>
	VECTOR SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom);

	/// <summary>
	/// 周囲の升が存在するか探す
	/// </summary>
	VECTOR SearchAroundSquares();

	/// <summary>
	/// 升のスコアを取得する
	/// </summary>
	/// <param name="direction">方向</param>
	/// <param name="index">升の番号</param>
	void ScoreCaluculation(Direction direction,int index);

	/// <summary>
	/// プレイヤーから敵までの直線距離の升にオブジェクトがあるか
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="enemyPos">敵の座標</param>
	/// <returns>直線距離にオブジェクトがあるか</returns>
	bool StraightLineDistanceSearch(VECTOR playerPos, VECTOR enemyPos);

	/// <summary>
	/// 座標から升のインデックスを取得する
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	int SearchCurrentIndex(VECTOR pos);

private:

	int enemyIndex_ = 0;
	int playerIndex_ = 0;
	int moveCount_ = 1;

	DesinationState destination_ = {};

	std::unordered_map<int, MasuState> masu_;
	std::unordered_map<Direction, Score> score_;
	std::unordered_map<Direction, Score> tempscore_;

	std::list<int> preteriteIndex_;

};

