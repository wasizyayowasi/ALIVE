#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>
#include <unordered_map>

class Aster;
class ShotManager;

class EnemyBase : public CharacterBase
{
public:
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	EnemyBase(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~EnemyBase() {};

	//更新
	void Update(Player& player, const InputState& input) override;
	//描画
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
	bool SearchForPlayer(VECTOR playerPos);

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

	/// <summary>
	/// 直線距離にオブジェクトがあるか
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>オブジェクトがあるか</returns>
	bool IsThereAnObject(VECTOR playerPos);

	/// <summary>
	/// 距離が範囲内だったら
	/// </summary>
	/// <returns></returns>
	bool DistanceIsWithinRange();

	void Shot(std::shared_ptr<ShotManager>shotManager, VECTOR playerPos, float height);

private:

	int fireFrameCount = 0;

	float distanceSize_ = 0.0f;							//敵からプレイヤーの距離
	float innerProduct = 0.0f;						//内積の結果を入れる

	bool isMove_ = false;							//移動中フラグ
	bool isDetection_ = false;						//プレイヤーを検知したか

	VECTOR frontVec_ = {};							//敵の正面ベクトルを入れる
	VECTOR pushVec_ = {};
	VECTOR shotMoveVec_ = {};

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

	std::shared_ptr<Aster> Aster_;


};

