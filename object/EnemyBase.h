#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>

class Aster;

class EnemyBase : public CharacterBase
{
public:
	//ファイルパスでモデルクラスをインスタンス化するコンストラクタ
	EnemyBase(const char* fileName, LoadObjectInfo objInfo);
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	EnemyBase(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~EnemyBase() {};

	void Update(Player& player) override;
	void Draw()override;

	/// <summary>
	/// プレイヤーを追跡する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void TrackingUpdate(VECTOR playerPos);

	/// <summary>
	/// プレイヤーを索敵する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void SearchForPlayer(VECTOR playerPos);

	/// <summary>
	/// プレイヤーを突き飛ばす
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void ThrustAway(Player& player);

	/// <summary>
	/// 経路探索
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void RoutingUpdate(Player& player);

	/*/// <summary>
	/// オブジェクトを回避しながら追跡する
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	void AvoidAndTrackObjectsUpdate(VECTOR playerPos);

	/// <summary>
	/// オブジェクトとの正面衝突をチェックする
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	void CheckHeadOnCollision();*/

private:

	float distance_ = 0.0f;				//敵からプレイヤーの距離
	float innerProduct = 0.0f;			//内積の結果を入れる

	bool isMove_ = false;				//移動中フラグ
	bool isHit_ = false;

	VECTOR frontVec_ = { 0.0f,0.0f ,-1.0f };		//敵の正面ベクトルを入れる
	VECTOR playerpos_ = { 0.0f,0.0f ,-1.0f };		//敵の正面ベクトルを入れる

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

	MV1_COLL_RESULT_POLY_DIM temp = {};

	std::shared_ptr<Aster> Aster_;

};

