#include "Effect2D.h"
#include <algorithm>

Effect2D::Effect2D(const char* const filepath, int numX, int numY, int graphSizeX, int graphSizeY,float size, VECTOR pos)
{
	//�����ǂݍ���
	LoadDivGraph(filepath, numX * numY, numX, numY, graphSizeX, graphSizeY, handle_, true);

	//���������c��
	totalNum_ = numX * numY;

	//���݂��Ă���t���O�����Ă�
	isEnabled_ = true;

	//�`��T�C�Y
	drawSize_ = size;

	//�`��ꏊ
	drawPos_ = pos;
}

Effect2D::~Effect2D()
{
	//�摜�̍폜
	for (auto& graph : handle_) {
		DeleteGraph(graph);
	}
}

void Effect2D::Update()
{
	//�摜�̍X�V
	currentNum_ = (std::min)(currentNum_ + 1, 60);

	//���݂̔ԍ����摜�̖����Ɠ����ɂȂ����瑶�݂�����
	if (currentNum_ == totalNum_) {
		isEnabled_ = false;
	}
}

void Effect2D::DrawBillboard()
{
	DrawBillboard3D(drawPos_, 0.5f, 0.5f, drawSize_, 0.0f, handle_[currentNum_], true);
}
