#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}

//�쐬�ς݂̃��f�����R�s�[����R���X�g���N�^
GimmickBase::GimmickBase(int handle,Material materialType, LoadObjectInfo objInfo): OrnamentBase(handle, materialType,objInfo)
{
}

//�f�X�g���N�^
GimmickBase::~GimmickBase()
{
}