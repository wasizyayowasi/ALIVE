#pragma once
#include "DxLib.h"
#include <memory>

class Camera;

class Broom
{
public:

	Broom();
	virtual ~Broom();

	void writingScreenUpdate(VECTOR playerPos);
	void graphFilterUpdate();

	void update();
	void draw();


private:

	int normalScreen_;			//���ʂ̕`�挋�ʂ��������ނ��߂̃X�N���[��
	int highBrightScreen_;		//���ʂ̕`�挋�ʂ��獂�P�x�����𔲂��o���ď������ރX�N���[��
	int reductioScaleScreen_;	//���P�x�������k���������ʂ��������ރX�N���[��
	int gaussScreen_;			//�k���摜���K�E�X�t�B���^�łڂ��������ʂ��������ރX�N���[��
	int gaussRation_;			//�K�E�X�t�B���^�̂ڂ����x����
	int reductionScale_;		//�k������T�C�Y
	int reductionScreenWidth_;	//�K�E�X�t�B���^���|����摜�̉���
	int reductionScreenHeight_;	//�K�E�X�t�B���^���~�摜�̏c��
	
	std::shared_ptr<Camera> camera_;

};

