#pragma once
#include "GimmickBase.h"
#include <vector>

class Player;

class Switch : public GimmickBase
{
public:
	//コンストラクタ
	Switch(const char* const filename);
	Switch(int handle);
	//デストラクタ
	virtual ~Switch();

	//更新
	void update(Player& player);

	VECTOR deadPersonCapsulePosAdjustment(std::shared_ptr<Model> person);

private:
	//スイッチモデルとプレイヤー、死体との衝突判定を行う
	void hitColl(Player& player);
	//スイッチをオンにする
	void OnAnim();
	//スイッチをオフにする
	void OffAnim();
private:

	std::vector<MV1_COLL_RESULT_POLY_DIM> hitDim_;

};

