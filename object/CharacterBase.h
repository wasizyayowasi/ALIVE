#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Enemy;

class CharacterBase
{
	friend Enemy;
public:
	CharacterBase(const char* fileName);
	CharacterBase(int modelHandle);
	virtual ~CharacterBase() {};

	virtual void update();
	virtual void draw();

	const VECTOR getPos() { return pos_; }

protected:

	int animNo_;			//�A�j���[�V�����ԍ�

	bool isAnimLoop_;		//�A�j���[�V���������[�v�Đ������邩

	VECTOR pos_;			//�|�W�V����
	VECTOR rot_;			//��]

	std::shared_ptr<Model> model_;

};

