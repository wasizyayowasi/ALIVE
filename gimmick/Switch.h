#pragma once
#include "../object/ObjectData.h"
#include <DxLib.h>
#include <list>
#include <memory>

class Player;
class ObjectBase;
class Model;

class Switch
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objInfo">配置データ</param>
	Switch( LoadObjectInfo objInfo);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Switch();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// スイッチモデルと死体の衝突判定を行う
	/// </summary>
	/// <param name="deadPerson">死体のポインタ</param>
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	/// <summary>
	/// 衝突判定の結果
	/// </summary>
	/// <returns>true：衝突　false：衝突していない</returns>
	bool CollResult();

	/// <summary>
	/// 衝突判定を行うモデルを追加する
	/// </summary>
	/// <returns>衝突判定を行うモデルのポインタ</returns>
	std::shared_ptr<Model> GetModelPointer();

	/// <summary>
	/// 衝突結果の削除
	/// </summary>
	void DeleteHitResult();

private:
	/// <summary>
	/// スイッチモデルとプレイヤーの衝突判定を行う
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void HitCollPlayer(Player& player);

	/// <summary>
	/// スイッチをオンにする
	/// </summary>
	void OnAnim();

	/// <summary>
	/// スイッチをオフにする
	/// </summary>
	void OffAnim();
private:

	VECTOR pos_ = {};									//ポジション

	std::shared_ptr<Model> model_;						//モデルクラスのスマートポインタ
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;		//衝突判定の配列
};

