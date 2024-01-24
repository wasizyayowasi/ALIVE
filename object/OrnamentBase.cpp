#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(int handle, LoadObjectInfo objInfo) : ObjectBase(handle,objInfo)
{
	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}