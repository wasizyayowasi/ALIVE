#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class CharacterBase
{
public:
	CharacterBase() {}
	virtual ~CharacterBase() {};

	virtual void update() = 0;
	virtual void draw() = 0;
private:

	int animNo_;			//�A�j���[�V�����ԍ�

	bool isAnimLoop_;		//�A�j���[�V���������[�v�Đ������邩

	VECTOR pos_;			//�|�W�V����
	VECTOR rot_;			//��]

	std::shared_ptr<Model> model_;

};

