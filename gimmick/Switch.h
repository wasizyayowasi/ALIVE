#pragma once
#include "GimmickBase.h"
#include <vector>

class Player;

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

private:
	//スイッチモデルとプレイヤー、死体との衝突判定を行う
	void HitColl(Player& player);
	//スイッチをオンにする
	void OnAnim();
	//スイッチをオフにする
	void OffAnim();
private:

	std::vector<MV1_COLL_RESULT_POLY_DIM> hitDim_;

};

