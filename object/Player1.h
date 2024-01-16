#pragma once
#include "PlayerData.h"
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>

class InputState;
class Model;
class ObjectManager;
class ObjectBase;
class ManualCrank;
class Lever;

template <class TemplateState> class StatePattern;

class Player1
{
public:

	enum class State {
		Normal,
		Jump,
		Death,
		Sit,
		Crank,
		Lever,
	};

	//update�֐�
	//�ʏ�X�V
	void NormalUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//�ړ�����
	void Move(const InputState& input);

	/// <summary>
	/// ����W�����v�ł͂Ȃ��Ƃ��̃W�����v
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	void JumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �v���C���[�̎��̂ɗ^����������֐�
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	void DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//�����Ă���r��
	void IdleToSitup(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// �v���C���[�ɍ���A�j���[�V������������֐�
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	void SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);


	//�N�����N����
	void GoCrankRotationPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	void CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// �������Ƃ̏Փ˃A�b�v�f�[�g
	/// </summary>
	void BulletHitMeUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//���o�[��|��
	void GoLeverPullPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	void LeverUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

private:
	//�ҋ@����
	void ChangeAnimIdle();
	//�ړ��x�N�g���Ɖ�]�̐���
	float ControlOfTranslationVecAndRot(const InputState& input);
	//��]����
	void RotationUpdate();
	//���̂̌㏈��
	void DeathPersonPostProsessing(std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
	/// </summary>
	void DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager);
	//�����^��
	void CarryObjectUpdate();
	//�ו������낷
	void DropOffObjectUpdate();
	/// <summary>
	/// �N�����N����]������A�b�v�f�[�g
	/// </summary>
	void CrankRotationUpdate(float rotZ);
};

