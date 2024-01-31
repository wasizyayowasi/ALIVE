#pragma once
#include "EnemyBase.h"

class ThrowEnemy : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// /// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">配置データ</param>
	ThrowEnemy(int handle, LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ThrowEnemy();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーを索敵する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void SearchForPlayer(VECTOR playerPos);

	/// <summary>
	/// 敵が弾を撃つ処理
	/// </summary>
	/// <param name="shotManager">弾を管理するクラスのポインタ</param>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="height">プレイヤーの高さ</param>
	void Shot(std::shared_ptr<ShotManager> shotManager, VECTOR playerPos, float height);

private:
	float innerProduct = 0.0f;						//内積の結果を入れる

	bool isDetection_ = false;						//プレイヤーを検知したか
	bool isThrow_ = false;							//投げるモーション中か

	VECTOR pushVec_ = {};							//プレイヤーが敵にぶつかったときに押すベクトル

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;
};

