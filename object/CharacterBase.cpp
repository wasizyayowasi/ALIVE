#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

CharacterBase::CharacterBase(const char* fileName):ObjectBase(fileName)
{
}

CharacterBase::CharacterBase(int handle) :ObjectBase(handle)
{
}

void CharacterBase::update()
{
}

void CharacterBase::draw()
{
}
