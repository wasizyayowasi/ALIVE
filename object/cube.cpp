#include "cube.h"
#include "../util/Model.h"

cube::cube():CarryObjectBase("data/model/box.mv1")
{
	pos_ = VGet(-100.0f, 0.0f, 0.0f);
	object_->setPos(pos_);
	object_->setCollFrame();
}

cube::~cube()
{
}

