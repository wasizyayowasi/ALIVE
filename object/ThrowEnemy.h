#pragma once
#include "EnemyBase.h"

class ThrowEnemy : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	ThrowEnemy(const int handle, const Material materialType, const LoadObjectInfo objInfo);

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
	/// 敵が弾を撃つ処理
	/// </summary>
	/// <param name="shotManager">弾を管理するクラスのポインタ</param>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="height">プレイヤーの高さ</param>
	void Shot(std::shared_ptr<ShotManager> shotManager, VECTOR playerPos, float height);

private:
	bool isFakeThrow_ = false;						//投げるふりをするか
};

