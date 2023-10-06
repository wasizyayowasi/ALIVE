#pragma once
#include "../object/OrnamentBase.h"

constexpr int max_hit_coll = 2048;

class GimmickBase : public OrnamentBase
{
public:
	//�t�@�C���p�X�Ń��f�����쐬����R���X�g���N�^
	GimmickBase(const char* filename);
	//�쐬�ς݂̃��f�����R�s�[����R���X�g���N�^
	GimmickBase(int handle);
	//�f�X�g���N�^
	virtual ~GimmickBase();

	//�X�V
	virtual void update(Player& player)override;
	//�`��
	void draw();

	std::shared_ptr<Model> getModelInfo() { return model_; }

private:

};

