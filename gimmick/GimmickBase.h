#pragma once
#include "../object/OrnamentBase.h"

class GimmickBase : public OrnamentBase
{
public:
	//�t�@�C���p�X�Ń��f�����쐬����R���X�g���N�^
	GimmickBase(const char* filename, LoadObjectInfo objInfo);
	//�쐬�ς݂̃��f�����R�s�[����R���X�g���N�^
	GimmickBase(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~GimmickBase();

	//�X�V
	virtual void Update(Player& player)override;
	//�`��
	void Draw();

	//�Փ˔���
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

	std::shared_ptr<Model> GetModelInfo() { return model_; }

protected:

	bool isTransit_ = false;

};

