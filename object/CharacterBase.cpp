#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

CharacterBase::CharacterBase(const char* fileName)
{
	model_ = std::make_shared<Model>(fileName);
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
