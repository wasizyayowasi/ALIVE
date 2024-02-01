#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(int handle, Material materialType, LoadObjectInfo objInfo) : ObjectBase(handle, materialType,objInfo)
{
	//衝突判定用フレームの設定
	model_->SetUseCollision(true, false);

	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}