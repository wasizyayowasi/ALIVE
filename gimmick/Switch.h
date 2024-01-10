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
	//�R���X�g���N�^
	Switch( LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~Switch();

	//�X�V
	void Update(Player& player);
	//�`��
	void Draw();

	//�X�C�b�`���f���Ǝ��̂Ƃ̏Փ˔�����s��
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//�Փ˔���̌��ʂ̏���
	bool CollResult();

	//�Փ˔�����s�����f����ǉ�����
	std::shared_ptr<Model> GetModelPointer();

	//�|�W�V�����̒ǉ�
	VECTOR GetPos() { return pos_; }

	//�Փˌ��ʂ̃N���A
	void DeleteHitResult();

private:
	//�X�C�b�`���f���ƃv���C���[�̏Փ˔�����s��
	void HitCollPlayer(Player& player);
	//�X�C�b�`���I���ɂ���
	void OnAnim();
	//�X�C�b�`���I�t�ɂ���
	void OffAnim();
private:

	bool isTransit_ = false;							//���߃t���O

	VECTOR pos_ = {};									//�|�W�V����

	std::shared_ptr<Model> model_;						//���f���N���X�̃X�}�[�g�|�C���^
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;		//�Փ˔���̔z��
};

