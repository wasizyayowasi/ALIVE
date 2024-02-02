#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>
#include <unordered_map>

class Aster;
class ShotManager;

//エネミーのアニメーションタイプ
enum class EnemyAnimType {
	Idle,
	Walk,
	Run,
	Death,
	Throw,
	max,
};

class EnemyBase : public CharacterBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	EnemyBase(int handle, Material materialType, LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBase() {};

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// プレイヤーを追跡する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void TrackingUpdate(VECTOR playerPos);

	/// <summary>
	/// プレイヤーを索敵する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void SearchForPlayer(VECTOR playerPos);

	/// <summary>
	/// プレイヤーを突き飛ばす
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void ThrustAway(Player& player);

	/// <summary>
	/// 経路探索
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void RoutingUpdate(Player& player);

	/// <summary>
	/// 直線距離にオブジェクトがあるか
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>オブジェクトがあるか</returns>
	bool IsThereAnObject(VECTOR playerPos);

	/// <summary>
	/// 敵が弾を撃つ処理
	/// </summary>
	/// <param name="shotManager">弾を管理するクラスのポインタ</param>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="height">プレイヤーの高さ</param>
	void Shot(std::shared_ptr<ShotManager>shotManager, VECTOR playerPos, float height);

	/// <summary>
	/// 回転行列と拡縮行列の合成
	/// </summary>
	/// <returns>回転行列と拡縮行列を合成した結果</returns>
	MATRIX CombiningRotAndScallMat(VECTOR distance);

protected:

	bool isDetection_ = false;						//プレイヤーを検知したか
	bool isThrow_ = false;							//投げるモーション中か

	VECTOR frontVec_ = {};							//敵の正面ベクトルを入れる
	VECTOR pushVec_ = {};							//プレイヤーが敵にぶつかったときに押すベクトル

	std::shared_ptr<Aster> Aster_;
};

