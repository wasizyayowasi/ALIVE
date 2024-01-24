#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson final : public CharacterBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">配置データ</param>
	/// <param name="animNo">アニメーション番号</param>
	DeadPerson(int handle, LoadObjectInfo objInfo, int animNo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DeadPerson();

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

	MV1_COLL_RESULT_POLY_DIM hitDim_ = {};

};

