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
	CrankScaffold(int handle, Material materialType, LoadObjectInfo objInfo);
	
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
	void Draw()override;

	/// <summary>
	/// 衝突判定を行うモデルの追加
	/// </summary>
	/// <returns>モデルポインタ</returns>
	std::shared_ptr<Model> AddCollModel();
private:

	float oldRotZ_ = 0.0f;
	float upVec_ = 0.0f;						//上昇ベクトル

	VECTOR initPos_ = {};						//初期ポジション
	VECTOR upperLimitPos_ = {};						//初期ポジション
	VECTOR lowerLimitPos_ = {};						//初期ポジション

	std::shared_ptr<ManualCrank> crank_;		//クランククラスのスマートポインタ

};

