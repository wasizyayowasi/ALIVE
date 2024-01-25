#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(int handle, LoadObjectInfo objInfo) : ObjectBase(handle,objInfo)
{
	//Õ“Ë”»’è—pƒtƒŒ[ƒ€‚ÌÝ’è
	model_->SetUseCollision(true, false);

	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}