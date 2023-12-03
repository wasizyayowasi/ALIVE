#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

CharacterBase::CharacterBase(int handle, LoadObjectInfo objInfo) :ObjectBase(handle,objInfo)
{
}

void CharacterBase::Update(Player& player, const InputState& input)
{
}

void CharacterBase::Draw()
{
}