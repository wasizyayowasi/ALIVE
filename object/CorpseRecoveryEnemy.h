#pragma once
#include "EnemyBase.h"
#include <DxLib.h>

class CorpseRecoveryEnemy : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">配置データ</param>
	CorpseRecoveryEnemy(int handle, LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CorpseRecoveryEnemy();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 死体を探す
	/// </summary>
	/// <param name="CorpsePos">死体のポジション</param>
	void SearchForCorpse(VECTOR CorpsePos);

	/// <summary>
	/// 死体に対しての更新
	/// </summary>
	/// <param name="pointer">死体のポインター</param>
	void UpdateForCorpse(std::shared_ptr<ObjectBase> pointer)override;

private:

	VECTOR initPos_ = {};			//初期ポジション

};

