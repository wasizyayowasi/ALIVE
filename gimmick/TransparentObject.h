#pragma once
#include "GimmickBase.h"
#include <memory>

class Switch;

class TransparentObject : public GimmickBase
{
public:
	//�R���X�g���N�^
	TransparentObject(const char* const filename, LoadObjectInfo objInfo);
	TransparentObject(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~TransparentObject();

	//�X�V
	void Update(Player& player)override;
	//�`��
	void Draw();

	//�V�����f���Ǝ��̂Ƃ̏Փ˔�����s��
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

private:

	float alphaValue_ = 0.0f;

	VECTOR InitialPosition_ = {};						//�����̍������擾����

	std::shared_ptr<Switch> switch_;

};

