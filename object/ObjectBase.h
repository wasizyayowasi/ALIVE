#pragma once
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>
#include <string>

class Model;
class Player;

class ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	ObjectBase(int handle,Material materialType, LoadObjectInfo objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ObjectBase();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	virtual void Update(Player& player);
	
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ���̂ɑ΂��Ă̍X�V
	/// </summary>
	/// <param name="pointer">���̂̃|�C���^</param>
	virtual void UpdateForCorpse(std::shared_ptr<ObjectBase> pointer);

	/// <summary>
	/// �|�W�V�����̎擾
	/// </summary>
	/// <returns>�|�W�V����</returns>
	VECTOR GetPos() { return pos_; }

	/// <summary>
	/// ���݁A���̃I�u�W�F�N�g���L����������
	/// </summary>
	/// <returns></returns>
	bool IsEnabled() { return isEnable_; };

	/// <summary>
	/// �Փ˔�����s�����擾����
	/// </summary>
	/// <returns>�Փ˔�����s����</returns>
	bool IsCollCheck() { return isCollCheck_; }

	/// <summary>
	/// �I�u�W�F�N�g���^��ł��邩�ǂ����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g���^��ł��邩</returns>
	bool IsTransit() { return isTransit_; }

	/// <summary>
	/// �����^�ђ��t���O�̐ݒ�
	/// </summary>
	/// <param name="isTransit">true�F�^��ł���@false�F�^��ł��Ȃ�</param>
	void SetIsTransit(bool isTransit) { isTransit_ = isTransit; }

	//�L���ȃI�u�W�F�N�g�������ȃI�u�W�F�N�g����ݒ肷��
	
	/// <summary>
	/// ���݁A���̃I�u�W�F�N�g���L����������
	/// </summary>
	/// <param name="isEnable">true�F�L���@false�F����</param>
	void SetIsEnable(bool isEnable) { isEnable_ = isEnable; }

	/// <summary>
	/// ���f���̃X�}�[�g�|�C���^���擾����
	/// </summary>
	/// <returns>���f���|�C���^</returns>
	std::shared_ptr<Model> GetModelPointer() { return model_; }

	/// <summary>
	/// �Փ˔�����s�����f����ǉ�����
	/// </summary>
	/// <returns>���f���|�C���^</returns>
	virtual std::shared_ptr<Model> AddCollModel();

	/// <summary>
	/// �|�W�V������ݒ肷��
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	void SetPos(VECTOR pos) { pos_ = pos; }

protected:

	bool isEnable_ = false;						//�L����������
	bool isCollCheck_ = false;					//�Փ˔�����s����
	bool isTransit_ = false;					//�����^�ђ���

	VECTOR pos_ = { 0.0f,0.0f,0.0f };			//�|�W�V����
	VECTOR rot_ = { 0.0f,0.0f,0.0f };			//��]��
	VECTOR scale_ = { 0.5f,0.5f,0.5f };			//�g�k��

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^

};

