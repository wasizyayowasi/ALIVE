#include "GimmickBase.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	constexpr float collition_radius = 200.0f;
}
//�t�@�C���p�X�Ń��f�����쐬����R���X�g���N�^
GimmickBase::GimmickBase(const char* filename):OrnamentBase(filename)
{
}

//�쐬�ς݂̃��f�����R�s�[����R���X�g���N�^
GimmickBase::GimmickBase(int handle): OrnamentBase(handle)
{
}

//�f�X�g���N�^
GimmickBase::~GimmickBase()
{
}

//�X�V
void GimmickBase::update(Player& player)
{
}

//�`��
void GimmickBase::draw()
{
	model_->draw();
}
