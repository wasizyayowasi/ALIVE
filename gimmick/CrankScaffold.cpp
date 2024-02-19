#include "CrankScaffold.h"
#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

//�R���X�g���N�^
CrankScaffold::CrankScaffold(const int handle, const Material materialType, const LoadObjectInfo objInfo) : GimmickBase(handle, materialType, objInfo)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//objInfo�̖��O���疖���̐��l���擾����
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");

	//��L�̐��l�Ƒ������̕������A����������������擾����
	std::string name = StrUtil::GetConcatenateNumAndStrings("Crank", ".", num);

	//�N�����N�̔z�u�f�[�^���擾����
	auto info = file.GetSpecifiedGimmickInfo(initPos_, name.c_str());

	//�C���X�^���X��
	crank_ = std::make_shared<ManualCrank>(info);

	//����̏���̃|�W�V����
	name = StrUtil::GetConcatenateNumAndStrings("CrankUpperLimit", ".", num);
	VECTOR upperLimitPos = file.GetSpecifiedGimmickInfo(initPos_, name.c_str()).pos;

	//����̉����̃|�W�V����
	name = StrUtil::GetConcatenateNumAndStrings("CrankLowerLimit", ".", num);
	VECTOR lowerLimitPos = file.GetSpecifiedGimmickInfo(initPos_, name.c_str()).pos;

	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(upperLimitPos, lowerLimitPos);

	//�����|�W�V�����̏�����
	initPos_ = objInfo.pos;

	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	//�㏸�x�N�g���̏�����
	upVec_ = distanceSize / crank_->GetMaxRotZ();
}

//�f�X�g���N�^
CrankScaffold::~CrankScaffold()
{
}

//�X�V
void CrankScaffold::Update(Player& player)
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	//�N�����N�ƃv���C���[�̏Փ˔�����s��
	//�������Ă����ꍇ�v���C���[�ɓ������Ă����N���X�̃|�C���^�[��
	//�Z�b�g����
	if (crank_->HitCollPlayer(player)) {
		player.SetCrankPointer(crank_);
	}

	float nowRotZ = crank_->GetRotZ();

	if (oldRotZ_ != nowRotZ) {
		if (crank_->GetMaxRotZ() == crank_->GetRotZ() || crank_->GetRotZ() == 0) {
			if (!sound.CheckSoundFile("stopCrank")) {
				sound.Set3DSoundInfo(pos_, 1500.0f, "stopCrank");
				sound.PlaySE("stopCrank");
			}
		}
	}

	//�ړ�
	pos_.y = crank_->GetRotZ()* upVec_ + initPos_.y;

	//�|�W�V�����̃Z�b�g
	model_->SetPos(pos_);

	//Z��]�̒l���c���Ă���
	oldRotZ_ = crank_->GetRotZ();
}

//�`��
void CrankScaffold::Draw()
{
	//���f���̕`��
	model_->Draw();
	//�N�����N�̕`��
	crank_->Draw();
}