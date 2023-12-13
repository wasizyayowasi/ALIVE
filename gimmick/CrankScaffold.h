#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold : public GimmickBase
{
public:
	//�R���X�g���N�^
	CrankScaffold(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~CrankScaffold();

	//�X�V
	void Update(Player& player, const InputState& input)override;
	//�`��
	void Draw();

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();
private:

	float upVec_ = 0.0f;						//�㏸�x�N�g��

	VECTOR initPos_ = {};						//�����|�W�V����

	std::shared_ptr<ManualCrank> crank_;		//�N�����N�N���X�̃X�}�[�g�|�C���^

};

