#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

CharacterBase::CharacterBase(const char* fileName):ObjectBase(fileName)
{
}

CharacterBase::CharacterBase(int handle) :ObjectBase(handle)
{
}

void CharacterBase::Update(Player& player)
{
}

void CharacterBase::Draw()
{
}
