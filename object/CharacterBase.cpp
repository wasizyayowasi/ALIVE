#include "CharacterBase.h"
#include "../util/Model.h"
#include <cassert>

CharacterBase::CharacterBase(const int handle, const Material materialType, const LoadObjectInfo objInfo) :ObjectBase(handle,materialType,objInfo)
{
}