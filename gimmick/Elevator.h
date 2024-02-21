#pragma once
#include "GimmickBase.h"
#include <memory>
#include <vector>
#include <DxLib.h>
#include <unordered_map>

class Switch;
class Lever;
class Model;

class Elevator final : public GimmickBase
{
private:
	//エレベーターのアニメタイプ
	enum class ElevatorAnimType
	{
		openIdle,
		closeIdle,
		open,
		close,
		max,
	};

	//エレベーターの状態
	enum class ElevatorState
	{
		upper,
		lower,
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	Elevator(const int handle, const Material materialType, const LoadObjectInfo objInfo);
	
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
	virtual const std::shared_ptr<Model>& AddCollModel()const override;

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

	float moveVecY_ = 0.0f;								//Y軸の移動ヴェクトル
	float elapsedTime_ = 0.0f;							//経過時間

	bool isDeparture_ = false;							//エレベーターが出発しているかどうか
	bool isPlaySound_ = false;							//サウンドが再生中か
	bool isOnSwitch_ = false;							//スイッチを押しているか

	VECTOR targetPos_ = {};								//現在向かっているポジション

	ElevatorState state_ = {};							//エレベーターの状態

	std::unordered_map<ElevatorState, VECTOR> stopPos_;	//ストップポジション

	std::shared_ptr<Switch> switch_;					//スイッチクラスのスマートポインタ

	std::vector<std::shared_ptr<Lever>> levers_;		//レバークラスのスマートポインタのVector配列
};

