#pragma once
#include "CharacterBase.h"

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

