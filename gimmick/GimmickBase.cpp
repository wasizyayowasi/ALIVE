#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}

GimmickBase::GimmickBase(const char* filename)
{
	model_ = std::make_shared<Model>(filename);
}

GimmickBase::GimmickBase(int handle)
{
	model_ = std::make_shared<Model>(handle);
}

GimmickBase::~GimmickBase()
{
}

void GimmickBase::update()
{
}

void GimmickBase::draw()
{
	model_->draw();
}
