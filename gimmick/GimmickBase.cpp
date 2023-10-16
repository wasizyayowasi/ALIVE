#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}
//�t�@�C���p�X�Ń��f�����쐬����R���X�g���N�^
GimmickBase::GimmickBase(const char* filename, LoadObjectInfo objInfo):OrnamentBase(filename,objInfo)
{
}

//�쐬�ς݂̃��f�����R�s�[����R���X�g���N�^
GimmickBase::GimmickBase(int handle, LoadObjectInfo objInfo): OrnamentBase(handle,objInfo)
{
}

//�f�X�g���N�^
GimmickBase::~GimmickBase()
{
}

//�X�V
void GimmickBase::Update(Player& player)
{
}

//�`��
void GimmickBase::Draw()
{
	model_->Draw();
}
