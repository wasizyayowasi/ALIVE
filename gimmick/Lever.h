#pragma once
#include "../object/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

class Lever
{
private:

	enum class AnimType {
		on,
		off,
		max,
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="info">配置データ</param>
	Lever(const LoadObjectInfo info);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Lever();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>true : 当たっている  false : 当たっていない</returns>
	bool CollCheck(const VECTOR playerPos) const;

	/// <summary>
	/// モデルポインタの取得
	/// </summary>
	/// <returns>モデルのポインタ</returns>
	std::shared_ptr<Model> GetModelPointer() const { return model_; }
	
	/// <summary>
	/// レバーを引くポジションの取得
	/// </summary>
	/// <returns>レバーを引くポジション</returns>
	VECTOR GetStandingPosition() const { return standingPos_; }

	/// <summary>
	/// レバーの起動
	/// </summary>
	void OnAnimation();

	/// <summary>
	/// レバーの停止
	/// </summary>
	void OffAnimation();

	/// <summary>
	/// レバーが起動しているかどうか取得
	/// </summary>
	/// <returns>レバーが起動しているか</returns>
	bool IsOn() const { return isOn_; }
private:

	bool isOn_ = false;							//レバーが起動しているかどうか

	VECTOR pos_ = {};							//ポジション
	VECTOR standingPos_ = {};					//エレベータの停止ポジション

	std::shared_ptr<Model> model_;				//モデルクラスのスマートポインタ

};

