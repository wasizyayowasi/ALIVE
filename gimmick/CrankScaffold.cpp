#include "CrankScaffold.h"
#include "ManualCrank.h"

#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"

#include "../object/Player.h"

namespace {
	constexpr float ascent_limit = 200.0f;
}

//�R���X�g���N�^
CrankScaffold::CrankScaffold(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	//�����|�W�V�����̏�����
	initPos_ = objInfo.pos;

	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	//objInfo�̖��O���疖���̐��l���擾����
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");
	//��L�̐��l�Ƒ������̕������A����������������擾����
	std::string name = StrUtil::GetConcatenateNumAndStrings("Crank", ".", num);

	//�N�����N�̔z�u�f�[�^���擾����
	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(initPos_, name.c_str());
	//�C���X�^���X��
	crank_ = std::make_shared<ManualCrank>(info);

	//�㏸�x�N�g���̏�����
	upVec_ = ascent_limit / crank_->GetMaxRotZ();
}

//�f�X�g���N�^
CrankScaffold::~CrankScaffold()
{
}

//�X�V
void CrankScaffold::Update(Player& player)
{
	//�N�����N�ƃv���C���[�̏Փ˔�����s��
	//�������Ă����ꍇ�v���C���[�ɓ������Ă����N���X�̃|�C���^�[��
	//�Z�b�g����
	if (crank_->HitCollPlayer(player)) {
		player.SetCrankPointer(crank_);
	}

	//�ړ�
	pos_.y = crank_->GetRotZ()* upVec_ + initPos_.y;

	//�|�W�V�����̃Z�b�g
	model_->SetPos(pos_);
}

//�`��
void CrankScaffold::Draw()
{
	//���f���̕`��
	model_->Draw();
	//�N�����N�̕`��
	crank_->Draw();
}

//�Փ˔�����s�����f���̒ǉ�
std::shared_ptr<Model> CrankScaffold::AddCollModel()
{
	return nullptr;
}
