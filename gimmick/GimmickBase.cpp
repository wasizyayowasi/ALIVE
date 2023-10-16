#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}
//ファイルパスでモデルを作成するコンストラクタ
GimmickBase::GimmickBase(const char* filename, LoadObjectInfo objInfo):OrnamentBase(filename,objInfo)
{
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
}
