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
	Steelyard(int handle, LoadObjectInfo objInfo);
	virtual ~Steelyard();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	//�V�����f���ƃv���C���[�̏Փ˔�����s��
	void HitCollPlayer(Player& player);
	//�V�����f���Ǝ��̂Ƃ̏Փ˔�����s��
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//���X�g�Ɍ��ʂ��܂Ƃ߂�
	void SummarizeList(MV1_COLL_RESULT_POLY_DIM hitDim,std::list<HitState>& list,bool& isStart);

	//�ŗD��ŏ������s��state���擾����
	void PriorityState(std::list<HitState> list,HitState& hitState);

	//frame�̈ړ�
	void MoveFrame(HitState LeftHitState, HitState RightHitState, float& moveDescent, bool isStart);
private:

	int time_ = 0; 

	float moveDescent_ = 0.0f;
	float playerHeight_ = 0.0f;

	bool isStart_ = false;

	VECTOR InitialPosition_ = {};						//�����̍������擾����
	VECTOR framePos_ = {};								//�����̍������擾����
	VECTOR playerPos_ = {};								//�����̍������擾����

	HitState priorityStateLeft_;						//�D�悳����X�e�[�^�X���擾����
	HitState priorityStateRight_;						//�D�悳����X�e�[�^�X���擾����
	HitState moveState_;

	MV1_COLL_RESULT_POLY_DIM hitDimRight_;				//�Փ˔���̌��ʂ����W
	MV1_COLL_RESULT_POLY_DIM hitDimLeft_;				//�Փ˔���̌��ʂ����W
	std::list<HitState> hitStateListLeft_;				//�Փ˂������̃X�e�[�^�X�����W
	std::list<HitState> hitStateListRight_;				//�Փ˂������̃X�e�[�^�X�����W

};

