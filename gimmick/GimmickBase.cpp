#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}
//ファイルパスでモデルを作成するコンストラクタ
GimmickBase::GimmickBase(const char* filename)
{
	model_ = std::make_shared<Model>(filename);
}

//作成済みのモデルをコピーするコンストラクタ
GimmickBase::GimmickBase(int handle)
{
	model_ = std::make_shared<Model>(handle);
}

//デストラクタ
GimmickBase::~GimmickBase()
{
}

//更新
void GimmickBase::update()
{
}

//描画
void GimmickBase::draw()
{
	model_->draw();
}
