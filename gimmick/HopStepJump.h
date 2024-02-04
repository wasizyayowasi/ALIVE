#pragma once
#include "GimmickBase.h"

class HopStepJump : public GimmickBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	HopStepJump(int handle, Material materialType, LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~HopStepJump();

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

	int currentNum_ = 0;			//現在の番号

	int materialNum_ = 0;			//マテリアルの数

	float elapseddTime_ = 0.0f;		//経過時間

	float alphaValue_ = 0.0f;		//透過値

};

