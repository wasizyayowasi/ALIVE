#pragma once
#include "GimmickBase.h"
#include <memory>

class Switch;

class TransparentObject : public GimmickBase
{
public:
	//�R���X�g���N�^
	TransparentObject(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~TransparentObject();

	//�X�V
	void Update(Player& player)override;
	//�`��
	void Draw();

	//�V�����f���Ǝ��̂Ƃ̏Փ˔�����s��
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();

private:

	int materialNum_ = 0;								//���̃N���X�ŃC���X�^���X�������3D���f���̃}�e���A���̐���ۊǂ���

	float alphaValue_ = 0.0f;							//���ߒl��ۊǂ���

	std::shared_ptr<Switch> switch_;

};

