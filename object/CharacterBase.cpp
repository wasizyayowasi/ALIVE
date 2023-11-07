#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

CharacterBase::CharacterBase(const char* fileName, LoadObjectInfo objInfo):ObjectBase(fileName,objInfo)
{
}

CharacterBase::CharacterBase(int handle, LoadObjectInfo objInfo) :ObjectBase(handle,objInfo)
{
}

void CharacterBase::Update(Player& player, const InputState& input)
{
}

void CharacterBase::Draw()
{
}

void CharacterBase::HitColl(std::shared_ptr<ObjectBase> pointer)
{
}

std::shared_ptr<Model> CharacterBase::AddCollModel()
{
    return nullptr;
}
