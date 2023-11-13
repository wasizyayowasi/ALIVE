#pragma once
#include "GimmickBase.h"
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
	Switch(const char* const filename, LoadObjectInfo objInfo);
	Switch(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~Switch();

	//更新
	void Update(Player& player);
	void Draw();

	//スイッチモデルと死体との衝突判定を行う
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//衝突判定の結果の処理
	bool CollResult();

	//衝突判定を行うモデルを追加する
	std::shared_ptr<Model> GetModelPointer();

private:
	//スイッチモデルとプレイヤーの衝突判定を行う
	void HitCollPlayer(Player& player);
	//スイッチをオンにする
	void OnAnim();
	//スイッチをオフにする
	void OffAnim();
private:

	bool isTransit_ = false;

	VECTOR pos_ = {};

	std::shared_ptr<Model> model_;
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;
};

