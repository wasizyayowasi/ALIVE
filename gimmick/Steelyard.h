#pragma once
#include "GimmickBase.h"
#include <memory>
#include <list>
#include <string>

class Model;

enum class HitState {
	top,
	bottom,
	noHit,
};

class Steelyard : public GimmickBase
{
public:
	Steelyard(const char* const filename, LoadObjectInfo objInfo);
	Steelyard(int handle, LoadObjectInfo objInfo);
	virtual ~Steelyard();

	void Update(Player& player)override;
	void Draw();

	//�V�����f���ƃv���C���[�̏Փ˔�����s��
	void HitCollPlayer(Player& player);
	//�V�����f���Ǝ��̂Ƃ̏Փ˔�����s��
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//���X�g�Ɍ��ʂ��܂Ƃ߂�
	void SummarizeList(std::list<MV1_COLL_RESULT_POLY_DIM> hitDim,std::list<HitState>& list);

	//�ŗD��ŏ������s��state���擾����
	void PriorityState(std::list<HitState> list,HitState& hitState);

	//frame�̈ړ�
	void MoveFrame(HitState hitState,std::string string);

private:

	int time_ = 0;

	float moveDescent_ = 0.0f;
	float move_ = 0.0f;

	bool isStart_ = false;

	VECTOR InitialPosition_ = {};						//�����̍������擾����
	VECTOR framePos_ = {};						//�����̍������擾����

	HitState priorityStateLeft_;						//�D�悳����X�e�[�^�X���擾����
	HitState priorityStateRight_;						//�D�悳����X�e�[�^�X���擾����

	std::list<HitState> hitStateListLeft_;				//�Փ˂������̃X�e�[�^�X�����W
	std::list<HitState> hitStateListRight_;				//�Փ˂������̃X�e�[�^�X�����W
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDimLeft_;	//�Փ˔���̌��ʂ����W
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDimRight_;	//�Փ˔���̌��ʂ����W

};

