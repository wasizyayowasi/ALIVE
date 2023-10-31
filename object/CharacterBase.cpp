#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

CharacterBase::CharacterBase(const char* fileName, LoadObjectInfo objInfo):ObjectBase(fileName,objInfo)
{
}

CharacterBase::CharacterBase(int handle, LoadObjectInfo objInfo) :ObjectBase(handle,objInfo)
{
}

void CharacterBase::Update(Player& player)
{
}

void CharacterBase::Draw()
{
}

void CharacterBase::HitColl(std::shared_ptr<ObjectBase> pointer)
{
}
