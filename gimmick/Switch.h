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
	//コンストラクタ
	Switch( LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~Switch();

	//更新
	void Update(Player& player);
	//描画
	void Draw();

	//スイッチモデルと死体との衝突判定を行う
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//衝突判定の結果の処理
	bool CollResult();

	//衝突判定を行うモデルを追加する
	std::shared_ptr<Model> GetModelPointer();

	//ポジションの追加
	VECTOR GetPos() { return pos_; }

	//衝突結果のクリア
	void DeleteHitResult();

private:
	//スイッチモデルとプレイヤーの衝突判定を行う
	void HitCollPlayer(Player& player);
	//スイッチをオンにする
	void OnAnim();
	//スイッチをオフにする
	void OffAnim();
private:

	bool isTransit_ = false;							//透過フラグ

	VECTOR pos_ = {};									//ポジション

	std::shared_ptr<Model> model_;						//モデルクラスのスマートポインタ
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;		//衝突判定の配列
};

