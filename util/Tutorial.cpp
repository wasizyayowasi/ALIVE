#include "Tutorial.h"
#include "InputState.h"
#include "ExternalFile.h"
#include "FontsManager.h"
#include "DrawFunctions.h"
#include "Util.h"
#include "game.h"

#include <algorithm>

namespace {
	//�L�[�{�[�h�摜�̃`�b�v�T�C�Y
	constexpr int input_graph_chip_size = 59;

	//�p�b�h�摜�̃`�b�v�T�C�Y
	constexpr int controller_graph_chip_size = 59;

	//�ő厞��
	constexpr int max_time = 300;

	//�摜�T�C�Y�̊g�k��
	constexpr float graph_scale_size = 1.2f;

	//UI��\�����鍂���̕␳
	constexpr float correction_height = 1.3f;
}

//�R���X�g���N�^
Tutorial::Tutorial()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�L�[�{�[�h�̉摜��`�悷��ʒu
	UIPos_[UIGraph::KeyBord].x = Game::screen_width / 2 - input_graph_chip_size;
	UIPos_[UIGraph::KeyBord].y = Game::screen_height - input_graph_chip_size * correction_height;

	//pad�̃{�^����`�悷��ʒu
	UIPos_[UIGraph::XboxBotton].x = Game::screen_width / 2 - controller_graph_chip_size;
	UIPos_[UIGraph::XboxBotton].y = Game::screen_height - controller_graph_chip_size * correction_height;

	//�N�����N�̃`���[�g���A��
	tutorialData_["CrankTutorial"]	.push_back({ static_cast<int>(InputType::Activate)	,false , "�N�����N����" });
	tutorialData_["CrankTutorial"]	.push_back({ static_cast<int>(InputType::Max)		,false , "��" });
	tutorialData_["CrankTutorial"]	.push_back({ static_cast<int>(InputType::Activate)	,false , "����" });

	//���̂����Ă邱�Ƃɂ��Ẵ`���[�g���A��
	tutorialData_["SwitchTutorial"]	.push_back({ static_cast<int>(InputType::Death)		,false , "���S" });
	tutorialData_["SwitchTutorial"] .push_back({ static_cast<int>(InputType::Activate)	,false , "���̂�����" });
	tutorialData_["SwitchTutorial"] .push_back({ static_cast<int>(InputType::Activate)	,false , "���̂�u��" });

	//����`���[�g���A��
	tutorialData_["RunTutorial"]	.push_back({ static_cast<int>(InputType::Dush)		,false , "����" });

	//�W�����v�̃`���[�g���A��
	tutorialData_["JumpTutorial"]	.push_back({ static_cast<int>(InputType::Space)		,false , "�W�����v" });

	//�G���x�[�^�[�̃`���[�g���A��
	tutorialData_["ElevatorTutorial"].push_back({ static_cast<int>(InputType::Activate)	,false , "���o�[������" });

	//���̂𑫏�Ƃ��Ďg���`���[�g���A��
	tutorialData_["CorpseScaffoldTutorial"].push_back({ static_cast<int>(InputType::Death)	,false , "���S" });
	tutorialData_["CorpseScaffoldTutorial"].push_back({ static_cast<int>(InputType::Max)	,false , "���̂͑���Ƃ��Ďg����" });

	//�t�H���g�̎擾
	fontPigumo42_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
}

//�f�X�g���N�^
Tutorial::~Tutorial()
{
}

//�X�V
void Tutorial::Update(const VECTOR& playerPos)
{
	//�v���C���[�����ԋ߂��`���[�g���A���̔z�u�f�[�^���擾
	LoadObjectInfo tutorialInfo = ExternalFile::GetInstance().GetTutorialObjInfo(playerPos);

	//�擾�����f�[�^�̃|�W�V��������v���C���[�̃|�W�V�����̋����̃T�C�Y���擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorialInfo.pos, playerPos);

	//�擾�����f�[�^�̋����̃T�C�Y�𔼕��ɂ���
	float range = VSize(tutorialInfo.scale) / 2;

	//�`���[�g���A���͈͓̔��ɓ����Ă�����
	//��ԋ߂��`���[�g���A���̖��O���擾����
	if (range > distanceSize)
	{
		//��ԋ߂��`���[�g���A���̖��O���擾
		currentTutorialName_ = tutorialInfo.name;
	}

	//���݂̃`���[�g���A���̖��O��1�t���[���O�̃`���[�g���A���̖��O��
	//����Ă�����currentDisplayNum_��0�������
	if (currentTutorialName_ != oldTutorialName_)
	{
		currentDisplayNum_ = 0;
		elapsedTime_ = 0;
	}

	//1�t���[���O�̃`���[�g���A���Ɏg���Ă������O��ۑ�����
	oldTutorialName_ = currentTutorialName_;
}

//�`��
void Tutorial::Draw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�\������`���[�g���A���̐��𒴂�����ȍ~�����\�����Ȃ�
	//currentDisplayNum_��0����n�܂邽�ߐ�������1��ǉ�����
	if (currentDisplayNum_ + 1 > static_cast<int>(tutorialData_[currentTutorialName_].size()))
	{
		return;
	}

	//���݂̃`���[�g���A���f�[�^�̎擾
	TutorialData& data = tutorialData_[currentTutorialName_][currentDisplayNum_];

	//�L�[�^�C�v��\�����邩
	if (DoNotDiplayKeyType(data))
	{
		//������̉������擾����
		int width = GetDrawStringWidthToHandle(data.str.c_str(), static_cast<int>(data.str.size()), fontPigumo42_);

		//�L�[�̖����̕`��
		DrawStringToHandle(Game::screen_width / 2 - width / 2, Game::screen_height - static_cast<int>(input_graph_chip_size * 1.6f), data.str.c_str(), 0xffffff, fontPigumo42_);

		return;
	}

	//�L�[�̖����̕`��
	DrawStringToHandle(Game::screen_width / 2, static_cast<int>(Game::screen_height - input_graph_chip_size * 1.6f), data.str.c_str(), 0xffffff, fontPigumo42_);

	//�L�[�摜�̕`��
	input.DrawKeyGraph(data.keyType, UIPos_[UIGraph::KeyBord].x, UIPos_[UIGraph::KeyBord].y, graph_scale_size);

	//���ݕ\������Ă���L�[�������ꂽ��
	if (input.IsTriggered(static_cast<InputType>(data.keyType)))
	{
		data.isPushKey = true;
		currentDisplayNum_++;
	}
}

//�L�[�^�C�v��\�����Ȃ���
bool Tutorial::DoNotDiplayKeyType(const TutorialData& data)
{
	//�o�ߎ��Ԃ��ő厞�ԂƓ����������猻�݂̕\���ԍ��𑝂₷
	if (elapsedTime_ == max_time)
	{
		currentDisplayNum_++;
		elapsedTime_ = 0;
	}

	//key�^�C�v��max��������o�ߎ��Ԃ𑝂₵�A���^�[������
	if (static_cast<int>(InputType::Max) == data.keyType)
	{
		elapsedTime_ = (std::min)(elapsedTime_ + 1, max_time);
		return true;
	}

	return false;
}