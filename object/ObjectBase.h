#pragma once
#include <DxLib.h>
#include "../util/ObjectData.h"
#include <memory>

class Model;
class Player;
class InputState;

class ObjectBase
{
public:
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	ObjectBase(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~ObjectBase();

	//�X�V
	virtual void Update(Player& player,const InputState& input);
	//�`��
	virtual void Draw();

	//�Փ˔���
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

	//�|�W�V�����̎擾
	VECTOR GetPos() { return pos_; }

	//���ݗL�������������擾����
	bool IsEnabled() { return isEnable_; };

	//�Փ˔�����s����
	bool IsCollCheck() { return isCollCheck_; }

	//�����^�ђ���
	bool IsTransit() { return isTransit_; }

	//�����^�ђ��t���O�̐ݒ�
	void SetIsTransit(bool isTransit) { isTransit_ = isTransit; }

	//�L���ȃI�u�W�F�N�g�������ȃI�u�W�F�N�g����ݒ肷��
	void SetIsEnable(bool isEnable) { isEnable_ = isEnable; }

	//���f���̃X�}�[�g�|�C���^���擾����
	std::shared_ptr<Model> GetModelPointer() { return model_; }

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();

protected:

	bool isEnable_ = false;						//�L����������
	bool isCollCheck_ = false;					//�Փ˔�����s����
	bool isTransit_ = false;					//�����^�ђ���

	VECTOR pos_ = { 0.0f,0.0f,0.0f };			//�|�W�V����
	VECTOR rot_ = { 0.0f,0.0f,0.0f };			//��]��
	VECTOR scale_ = { 0.5f,0.5f,0.5f };			//�g�k��

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^

};

