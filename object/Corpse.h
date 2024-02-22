#pragma once
#include "CharacterBase.h"

class Model;

class Corpse final : public CharacterBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	/// <param name="animNo">アニメーション番号</param>
	Corpse(const int handle, const Material materialType, const LoadObjectInfo& objInfo,const int animNo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Corpse();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
};

