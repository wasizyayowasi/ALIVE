#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold final : public GimmickBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	CrankScaffold(int handle, Material materialType, LoadObjectInfo objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CrankScaffold();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �Փ˔�����s�����f���̒ǉ�
	/// </summary>
	/// <returns>���f���|�C���^</returns>
	std::shared_ptr<Model> AddCollModel();
private:

	float oldRotZ_ = 0.0f;
	float upVec_ = 0.0f;						//�㏸�x�N�g��

	VECTOR initPos_ = {};						//�����|�W�V����
	VECTOR upperLimitPos_ = {};						//�����|�W�V����
	VECTOR lowerLimitPos_ = {};						//�����|�W�V����

	std::shared_ptr<ManualCrank> crank_;		//�N�����N�N���X�̃X�}�[�g�|�C���^

};

