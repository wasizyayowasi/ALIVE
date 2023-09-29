#pragma once
#include "DxLib.h"
#include <memory>
#include <vector>
#include <list>
#include <deque>

class Player;
class Model;

constexpr int max_hit_coll = 2048;

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
	void checkCollisionPersonalArea(Player& player,VECTOR moveVec, std::list<std::shared_ptr<Model>> models);

	/// <summary>
	/// 衝突したモデルのポリゴンが壁かを判断する
	/// </summary>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="playerHeight">プレイヤーの高さ</param>
	void checkCollisionWall(VECTOR moveVec, float playerHeight);

	/// <summary>
	/// 衝突したモデルのポリゴンが床かを判断する
	/// </summary>
	/// <param name="player">プレイヤーを参照</param>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="jumpFlag">プレイヤーのジャンプフラグ</param>
	/// <param name="playerHeight">プレイヤーの高さ</param>
	void checkCollisionFloor(Player& player, VECTOR moveVec,bool jumpFlag,float playerHeight);

	/// <summary>
	/// checkCollisionPersonalArea、checkCollisionWall、checkCollisionFloorを呼び出す。
	/// 最後、上記の関数で取得した値を消去する
	/// </summary>
	/// <param name="player">プレイヤーを参照する</param>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="model">衝突判定を行うモデル</param>
	/// <param name="playerHeight">プレイヤーの高さ</param>
	/// <param name="isJump">プレイヤーのジャンプフラグ</param>
	/// <param name="jumpVec">プレイヤーのジャンプベクトル</param>
	void checkCollision(Player& player, VECTOR moveVec, std::list<std::shared_ptr<Model>> model,float playerHeight,bool isJump,float jumpVec);

	/// <summary>
	/// 衝突したオブジェクトが乗り越えられるか判断する
	/// </summary>
	/// <param name="playerHeight">プレイヤーの高さ</param>
	void checkStepDifference(Player& player, float playerHeight);

private:

	float objectHeightY = 0;		//衝突したオブジェクトの高さを保管する

	bool isGoUpStep_ = false;	//段差を上ることが出来る

	VECTOR oldPos;				//現在のプレイヤーの座標
	VECTOR nowPos;				//プレイヤーの移動量と現在の座標を足して結果
	int i = 0;
	int	j = 0;
	int	k = 0;
	int hitWallNum = 0;				//壁と衝突したポリゴンの数
	int hitFloorNum = 0;			//床と衝突したポリゴンの数
	bool moveFlag = false;		//現在移動しているかのフラグ
	bool hitFlag = false;		//衝突したかのフラグ

	//モデルとの当たり判定用メソッド
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;
	MV1_COLL_RESULT_POLY* wallHitDim_[max_hit_coll] = {};
	MV1_COLL_RESULT_POLY* floorHitDim_[max_hit_coll] = {};
	MV1_COLL_RESULT_POLY* hitPoly = {};
	HITRESULT_LINE hitLineResult = {};
};

