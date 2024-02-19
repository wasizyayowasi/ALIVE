#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold final : public GimmickBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	CrankScaffold(const int handle, const Material materialType, const LoadObjectInfo objInfo);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CrankScaffold();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;;
private:

	float oldRotZ_ = 0.0f;						//古いZの回転
	float upVec_ = 0.0f;						//上昇ベクトル

	VECTOR initPos_ = {};						//初期ポジション

	std::shared_ptr<ManualCrank> crank_;		//クランククラスのスマートポインタ

};

