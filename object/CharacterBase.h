#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	//�t�@�C���p�X�Ń��f���N���X���C���X�^���X������R���X�g���N�^
	CharacterBase(const char* fileName);
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	CharacterBase(int handle);
	//�f�X�g���N�^
	virtual ~CharacterBase() {};

	//�X�V
	virtual void update();
	//�`��
	virtual void draw();

protected:

	int animNo_ = 0;				//�A�j���[�V�����ԍ�

	bool isAnimLoop_ = false;		//�A�j���[�V���������[�v�Đ������邩

};

