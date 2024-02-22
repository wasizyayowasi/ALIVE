#pragma once
#include <DxLib.h>
#include <memory>
#include <list>

class Model;
class Shot;
class Player;

class ShotManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ShotManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ShotManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーと衝突したかチェックする
	/// </summary>
	/// <param name="player"></param>
	void Hit(Player& player);

	/// <summary>
	/// 弾を打つ
	/// </summary>
	/// <param name="framePos">初期ポジション</param>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="height">プレイヤーの高さ</param>
	void Fire(const VECTOR& framePos, const VECTOR& playerPos, const float height);

private:
	std::list<std::shared_ptr<Shot>> shots_;		//モデルポインタのリスト
};

