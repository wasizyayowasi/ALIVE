#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	CharacterBase(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~CharacterBase() {};

	//�X�V
	virtual void Update(Player& player, const InputState& input) override;
	//�`��
	virtual void Draw();

	//�Փ˔���
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();

protected:

	int animNo_ = 0;				//�A�j���[�V�����ԍ�

	bool isAnimLoop_ = false;		//�A�j���[�V���������[�v�Đ������邩

};

