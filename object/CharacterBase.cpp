#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

namespace {
	const VECTOR scale = { 0.5f,0.5f,0.5f };
}

CharacterBase::CharacterBase(const char* fileName)
{
	model_ = std::make_shared<Model>(fileName);
	model_->setScale(scale);
}

CharacterBase::CharacterBase(int modelHandle)
{
	assert(modelHandle != -1);

	model_ = std::make_shared<Model>(modelHandle);
}

void CharacterBase::update()
{
	model_->update();
}

void CharacterBase::draw()
{
	model_->draw();
}
