#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Switch;
class Steelyard;
class Player;

constexpr int max_hit_coll = 2048;

class GimmickBase
{
	friend Switch;
	friend Steelyard;
public:
	//�t�@�C���p�X�Ń��f�����쐬����R���X�g���N�^
	GimmickBase(const char* filename);
	//�쐬�ς݂̃��f�����R�s�[����R���X�g���N�^
	GimmickBase(int handle);
	//�f�X�g���N�^
	virtual ~GimmickBase();

	//�X�V
	virtual void update(Player& player);
	//�`��
	void draw();

	std::shared_ptr<Model> getModelInfo() { return model_; }

private:

	VECTOR pos_;		//�|�W�V����

	std::shared_ptr<Model> model_;		//3D���f��

};

