#pragma once
#include "GimmickBase.h"
#include <list>

class Player;
class ObjectBase;

class Switch : public GimmickBase
{
public:
	//�R���X�g���N�^
	Switch(const char* const filename, LoadObjectInfo objInfo);
	Switch(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~Switch();

	//�X�V
	void Update(Player& player)override;
	void Draw();

private:
	//�X�C�b�`���f���ƃv���C���[�̏Փ˔�����s��
	void HitCollPlayer(Player& player);
	//�X�C�b�`���f���Ǝ��̂Ƃ̏Փ˔�����s��
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);
	//�X�C�b�`���I���ɂ���
	void OnAnim();
	//�X�C�b�`���I�t�ɂ���
	void OffAnim();
private:
	int time = 0;
	int a = 0;

	bool hit = false;

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

};

