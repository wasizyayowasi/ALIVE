#pragma once
#include "GimmickBase.h"

class PenetrationScaffld final : public GimmickBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	PenetrationScaffld(int handle, Material materialType, LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PenetrationScaffld();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:

	int materialNum_ = 0;				//マテリアルの数

	float alphaValue_ = 1.0f;			//アルファ値

	bool alphaValueIncrease_ = false;	//アルファ値の増加
};

