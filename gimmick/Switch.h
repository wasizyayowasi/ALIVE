#pragma once
#include "GimmickBase.h"
#include <vector>

class Player;

class Switch : public GimmickBase
{
public:
	//�R���X�g���N�^
	Switch(const char* const filename);
	Switch(int handle);
	//�f�X�g���N�^
	virtual ~Switch();

	//�X�V
	void update(Player& player);

	VECTOR deadPersonCapsulePosAdjustment(std::shared_ptr<Model> person);

private:
	//�X�C�b�`���f���ƃv���C���[�A���̂Ƃ̏Փ˔�����s��
	void hitColl(Player& player);
	//�X�C�b�`���I���ɂ���
	void OnAnim();
	//�X�C�b�`���I�t�ɂ���
	void OffAnim();
private:

	std::vector<MV1_COLL_RESULT_POLY_DIM> hitDim_;

};

