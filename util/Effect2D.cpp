#include "Effect2D.h"
#include <algorithm>

Effect2D::Effect2D(std::vector<int> handle,float size, VECTOR pos)
{
	//�n���h���̎擾
	handle_ = handle;

	//���݂��Ă���t���O�����Ă�
	isEnabled_ = true;

	//�`��T�C�Y
	drawSize_ = size;

	//�`��ꏊ
	drawPos_ = pos;
}

Effect2D::~Effect2D()
{
}

void Effect2D::Update()
{
	//�摜�̍X�V
	currentNum_ = (std::min)(currentNum_ + 1, 60);

	//���݂̔ԍ����摜�̖����Ɠ����ɂȂ����瑶�݂�����
	if (currentNum_ == handle_.size() - 1) {
		isEnabled_ = false;
	}
}

void Effect2D::DrawBillboard()
{
	DrawBillboard3D(drawPos_, 0.5f, 0.5f, drawSize_, 0.0f, handle_[currentNum_], true);
}
