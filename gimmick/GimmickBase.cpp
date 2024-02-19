#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}

//作成済みのモデルをコピーするコンストラクタ
GimmickBase::GimmickBase(const int handle, const Material materialType, const LoadObjectInfo objInfo): OrnamentBase(handle, materialType,objInfo)
{
}

//デストラクタ
GimmickBase::~GimmickBase()
{
}