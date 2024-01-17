#pragma once
#include <DxLib.h>
#include <unordered_map>
#include <list>



class Aster
{
private:

	enum class MasuMode {
		normalMode,				//通常モード
		doneMode,				//済みモード
		blockadeMode,			//封鎖モード
	};

	enum class Direction {
		left,					//左
		topLeft,				//左上
		top,					//上
		topRight,				//右上
		right,					//右
		bottomRight,			//右下
		bottom,					//下
		bottomLeft,				//左下
	};

	struct MasuState {
		VECTOR centerPos;		//中心座標
		MasuMode masuMode;		//升のモード
		int x;					//2次元配列のX
		int z;					//2次元配列のY
	};

	struct Score {
		int moveCost;			//移動コスト
		int estimationCost;		//推定コスト
		int score;				//プレイヤーを追跡するうえで得点を付ける
		int currentIndex;		//移動先のインデックス
		int destinationIndex;	//移動先のインデックス
		Direction dir;			//方角
	};

	struct DesinationState {
		int index;
		int score;
	};

public:
	Aster(VECTOR pos);
	virtual ~Aster();

	//初期化
	void Init();

	//更新
	void Update();
	//描画
	void Draw();

	/// <summary>
	/// ポジション情報を元に配列の何番目に存在するか取得する
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="pos">ポジション</param>
	void LocationInformation(VECTOR playerPos,VECTOR enemyPos);

	/// <summary>
	/// 経路探索
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// 周囲の升が存在するか探す
	/// </summary>
	void SearchAroundSquares();

	/// <summary>
	/// 周囲の升を検索する
	/// </summary>
	void SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom);

	/// <summary>
	/// 升のスコアを取得する
	/// </summary>
	/// <param name="direction">方向</param>
	/// <param name="index">升の番号</param>
	void ScoreCaluculation(Direction direction,int index);

	/// <summary>
	/// 座標から升のインデックスを取得する
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	int SearchCurrentIndex(VECTOR pos);

	/// <summary>
	/// 目的地の座標を取得する
	/// </summary>
	/// <returns></returns>
	VECTOR GetDestinationCoordinates(VECTOR playerPos, VECTOR enemyPos);


	bool SearchBlockadeMode(VECTOR pos);
private:

	int enemyIndex_ = 0;
	int playerIndex_ = 0;
	int moveCount_ = 0;

	int tempIndex_ = 0;

	int count_ = 0;

	std::unordered_map<int, MasuState> masu_;					//移動範囲
	std::unordered_map<int, Score> scoreTable_;					//移動範囲の升にスコアをつける
	std::unordered_map<int, Score> debugScoreTable;				//デバッグ用のスコアテーブル(削除してよい)

	std::unordered_map<int, std::list<int>> preteriteIndex_;

	std::list<int> route_;

};

