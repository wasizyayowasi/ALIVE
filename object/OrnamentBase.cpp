#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(int handle, Material materialType, LoadObjectInfo objInfo) : ObjectBase(handle, materialType,objInfo)
{
	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}