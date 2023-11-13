#pragma once
#include "GimmickBase.h"
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
	Switch(const char* const filename, LoadObjectInfo objInfo);
	Switch(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~Switch();

	//�X�V
	void Update(Player& player);
	void Draw();

	//�X�C�b�`���f���Ǝ��̂Ƃ̏Փ˔�����s��
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//�Փ˔���̌��ʂ̏���
	bool CollResult();

	//�Փ˔�����s�����f����ǉ�����
	std::shared_ptr<Model> GetModelPointer();

private:
	//�X�C�b�`���f���ƃv���C���[�̏Փ˔�����s��
	void HitCollPlayer(Player& player);
	//�X�C�b�`���I���ɂ���
	void OnAnim();
	//�X�C�b�`���I�t�ɂ���
	void OffAnim();
private:

	bool isTransit_ = false;

	VECTOR pos_ = {};

	std::shared_ptr<Model> model_;
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;
};

