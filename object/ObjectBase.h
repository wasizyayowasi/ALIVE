#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Player;

class ObjectBase
{
public:
	//�t�@�C���p�X�Ń��f���N���X���C���X�^���X������R���X�g���N�^
	ObjectBase(const char* const filename);
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	ObjectBase(int handle);
	//�f�X�g���N�^
	virtual ~ObjectBase();

	//�X�V
	virtual void Update(Player& player);
	//�`��
	virtual void Draw();

	//���ݗL�������������擾����
	bool IsEnabled() { return isEnable_; };

	//���f���̃X�}�[�g�|�C���^���擾����
	std::shared_ptr<Model> GetModelPointer() { return model_; }

protected:

	bool isEnable_ = false;						//�L����������

	VECTOR pos_ = { 0.0f,0.0f,0.0f };			//�|�W�V����
	VECTOR rot_ = { 0.0f,0.0f,0.0f };			//��]��
	VECTOR scale_ = { 0.5f,0.5f,0.5f };			//�g�k��

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^

};

