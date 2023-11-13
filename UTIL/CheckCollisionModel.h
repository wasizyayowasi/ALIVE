#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include <list>
#include <deque>

class Player;
class Model;
class ObjectManager;

constexpr int max_hit_coll = 2048;

struct CollModelState {
	MV1_COLL_RESULT_POLY_DIM hitDim;
	std::shared_ptr<Model> model;
};

struct CollResultPoly {
	MV1_COLL_RESULT_POLY* hitDim;
	std::shared_ptr<Model> model;
};

class CheckCollisionModel
{
public:

	//コンストラクタ
	CheckCollisionModel();
	//デストラクタ
	virtual ~CheckCollisionModel();

	/// <summary>
	/// 自分から一定範囲のオブジェクトとの衝突判定を得る
	/// </summary>
	/// <param name="player">プレイヤーを参照</param>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="models">プレイヤーと衝突判定を行うモデル</param>
	void CheckCollisionPersonalArea(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// 衝突した壁と床のポリゴン数を数える
	/// </summary>
	void CheckWallAndFloor();

	/// <summary>
	/// 衝突したモデルのポリゴンが壁かを判断し、移動ベクトルを補正する
	/// </summary>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="playerHeight">プレイヤーの高さ</param>
	void CheckCollisionWall(std::shared_ptr<Player> player);

	/// <summary>
	/// 衝突したモデルのポリゴンが床かを判断する
	/// </summary>
	/// <param name="player">プレイヤーを参照</param>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="jumpFlag">プレイヤーのジャンプフラグ</param>
	/// <param name="playerHeight">プレイヤーの高さ</param>
	void CheckCollisionFloor(std::shared_ptr<Player> player);

	/// <summary>
	/// checkCollisionPersonalArea、checkCollisionWall、checkCollisionFloorを呼び出す。
	/// 最後、上記の関数で取得した値を消去する
	/// </summary>
	/// <param name="player">プレイヤーを参照する</param>
	void CheckCollision(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// 衝突したオブジェクトが乗り越えられるか判断する
	/// </summary>
	void CheckStepDifference(std::shared_ptr<Player> player);


	void CheckCollSpecificModel(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager);

private:

	float objectHeightY = 0;		//衝突したオブジェクトの高さを保管する

	bool isGoUpStep_ = false;	//段差を上ることが出来る

	VECTOR oldPos;				//現在のプレイヤーの座標
	VECTOR nowPos;				//プレイヤーの移動量と現在の座標を足して結果
	int hitWallNum = 0;			//壁と衝突したポリゴンの数
	int hitFloorNum = 0;		//床と衝突したポリゴンの数
	bool moveFlag = false;		//現在移動しているかのフラグ
	bool hitFlag = false;		//衝突したかのフラグ

	//モデルとの当たり判定用メソッド
	std::list<CollModelState> hitDim_;
	CollResultPoly wallHitDim_[max_hit_coll] = {};
	CollResultPoly floorHitDim_[max_hit_coll] = {};
	HITRESULT_LINE hitLineResult = {};
};

