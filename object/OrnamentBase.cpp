#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(int handle, Material materialType, LoadObjectInfo objInfo) : ObjectBase(handle, materialType,objInfo)
{
	//Õ“Ë”»’è—pƒtƒŒ[ƒ€‚ÌÝ’è
	model_->SetUseCollision(true, false);

	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}