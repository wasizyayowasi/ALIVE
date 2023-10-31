#pragma once
#include "GimmickBase.h"
#include <list>

class Player;
class ObjectBase;

class Switch : public GimmickBase
{
public:
	//コンストラクタ
	Switch(const char* const filename, LoadObjectInfo objInfo);
	Switch(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~Switch();

	//更新
	void Update(Player& player)override;
	void Draw();

private:
	//スイッチモデルとプレイヤーの衝突判定を行う
	void HitCollPlayer(Player& player);
	//スイッチモデルと死体との衝突判定を行う
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);
	//スイッチをオンにする
	void OnAnim();
	//スイッチをオフにする
	void OffAnim();
private:
	int time = 0;
	int a = 0;

	bool hit = false;

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

};

