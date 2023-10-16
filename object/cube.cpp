#include "cube.h"
#include "../util/Model.h"

cube::cube(const char* const filename, LoadObjectInfo objInfo):CarryObjectBase(filename,objInfo)
{
	pos_ = VGet(-100.0f, 0.0f, 0.0f);
	model_->SetPos(pos_);
	model_->SetCollFrame();
}

cube::cube(int handle, LoadObjectInfo objInfo):CarryObjectBase(handle,objInfo)
{
	pos_ = VGet(-100.0f, 0.0f, 0.0f);
	model_->SetPos(pos_);
	model_->SetCollFrame();
}

cube::~cube()
{
}

