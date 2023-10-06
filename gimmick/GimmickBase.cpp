#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}
//ファイルパスでモデルを作成するコンストラクタ
GimmickBase::GimmickBase(const char* filename):OrnamentBase(filename)
{
}

//作成済みのモデルをコピーするコンストラクタ
GimmickBase::GimmickBase(int handle): OrnamentBase(handle)
{
}

//デストラクタ
GimmickBase::~GimmickBase()
{
}

//更新
void GimmickBase::update(Player& player)
{
}

//描画
void GimmickBase::draw()
{
	model_->draw();
}
