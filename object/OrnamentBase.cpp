#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(int handle, LoadObjectInfo objInfo) : ObjectBase(handle,objInfo)
{
	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}