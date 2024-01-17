#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}

//作成済みのモデルをコピーするコンストラクタ
GimmickBase::GimmickBase(int handle, LoadObjectInfo objInfo): OrnamentBase(handle,objInfo)
{
}

//デストラクタ
GimmickBase::~GimmickBase()
{
}

//更新
void GimmickBase::Update(Player& player)
{
}

//描画
void GimmickBase::Draw()
{
	model_->Draw();
	DrawCapsule3D(pos_, VAdd(pos_, VGet(0, 50, 0)), 50, 32, 0x00ff00, 0x00ff00, true);
	//DrawSphere3D(pos_, 50, 32, 0xff0000, 0xff0000, true);
}