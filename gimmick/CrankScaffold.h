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
	CrankScaffold(const int handle, const Material materialType, const LoadObjectInfo objInfo);
	
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
	void Draw()override;;
private:

	float oldRotZ_ = 0.0f;						//�Â�Z�̉�]
	float upVec_ = 0.0f;						//�㏸�x�N�g��

	VECTOR initPos_ = {};						//�����|�W�V����

	std::shared_ptr<ManualCrank> crank_;		//�N�����N�N���X�̃X�}�[�g�|�C���^

};

