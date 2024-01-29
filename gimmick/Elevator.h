#pragma once
#include "GimmickBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Switch;
class Lever;
class Model;

class Elevator final : public GimmickBase
{
private:
	//エレベーターのアニメタイプ
	enum class ElevatorAnimType {
		openIdle,
		closeIdle,
		open,
		close,
		max,
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">配置データ</param>
	Elevator(int handle, LoadObjectInfo objInfo);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Elevator();

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
	/// <returns></returns>
	virtual std::shared_ptr<Model> AddCollModel();

private:

	/// <summary>
	/// エレベーターを移動させる
	/// </summary>
	void Move();

	/// <summary>
	/// 移動先のポジションを取得する
	/// </summary>
	void TargetPosition();

private:

	float moveVecY_ = 0.0f;							//Y軸の移動ヴェクトル
	float elapsedTime_ = 0.0f;

	bool isDeparture_ = false;						//エレベーターが出発しているかどうか

	VECTOR targetPos_ = {};							//現在向かっているポジション

	std::shared_ptr<Switch> switch_;				//スイッチクラスのスマートポインタ

	std::vector<std::shared_ptr<Lever>> levers_;	//レバークラスのスマートポインタのVector配列
};

