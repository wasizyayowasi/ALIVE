#pragma once
#include "EnemyBase.h"

class tempEnemy : public EnemyBase
{
public:
	tempEnemy(const char* const filename, LoadObjectInfo objInfo);
	tempEnemy(int handle, LoadObjectInfo objInfo);
	virtual ~tempEnemy();

	//更新
	void Update(Player& player) override;
	//描画
	void Draw();

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

private:

	float distance_ = 0.0f;				//敵からプレイヤーの距離
	float innerProduct = 0.0f;			//内積の結果を入れる

	VECTOR frontVector_ = { 0.0f,0.0f ,-1.0f };		//敵の正面ベクトルを入れる
};

