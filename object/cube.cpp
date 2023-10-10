#include "cube.h"
#include "../util/Model.h"

cube::cube(const char* const filename):CarryObjectBase(filename)
{
	pos_ = VGet(-100.0f, 0.0f, 0.0f);
	model_->SetPos(pos_);
	model_->SetCollFrame();
}

cube::cube(int handle):CarryObjectBase(handle)
{
	pos_ = VGet(-100.0f, 0.0f, 0.0f);
	model_->SetPos(pos_);
	model_->SetCollFrame();
}

cube::~cube()
{
}

