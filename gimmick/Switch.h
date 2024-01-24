#pragma once
#include "../object/ObjectData.h"
#include <DxLib.h>
#include <list>
#include <memory>

class Player;
class ObjectBase;
class Model;

class Switch
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="objInfo">�z�u�f�[�^</param>
	Switch( LoadObjectInfo objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Switch();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player);
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �X�C�b�`���f���Ǝ��̂̏Փ˔�����s��
	/// </summary>
	/// <param name="deadPerson">���̂̃|�C���^</param>
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	/// <summary>
	/// �Փ˔���̌���
	/// </summary>
	/// <returns>true�F�Փˁ@false�F�Փ˂��Ă��Ȃ�</returns>
	bool CollResult();

	/// <summary>
	/// �Փ˔�����s�����f����ǉ�����
	/// </summary>
	/// <returns>�Փ˔�����s�����f���̃|�C���^</returns>
	std::shared_ptr<Model> GetModelPointer();

	/// <summary>
	/// �Փˌ��ʂ̍폜
	/// </summary>
	void DeleteHitResult();

private:
	/// <summary>
	/// �X�C�b�`���f���ƃv���C���[�̏Փ˔�����s��
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void HitCollPlayer(Player& player);

	/// <summary>
	/// �X�C�b�`���I���ɂ���
	/// </summary>
	void OnAnim();

	/// <summary>
	/// �X�C�b�`���I�t�ɂ���
	/// </summary>
	void OffAnim();
private:

	VECTOR pos_ = {};									//�|�W�V����

	std::shared_ptr<Model> model_;						//���f���N���X�̃X�}�[�g�|�C���^
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;		//�Փ˔���̔z��
};

