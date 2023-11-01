#include "Steelyard.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };

	constexpr float move_speed = 0.02f;

}

Steelyard::Steelyard(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
	InitialPosition_ = pos_;
	isCollCheck_ = true;
}

Steelyard::Steelyard(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//��
	InitialPosition_ = pos_;
	isCollCheck_ = true;
}

Steelyard::~Steelyard()
{
}

void Steelyard::Update(Player& player)
{
	playerPos_ = player.GetStatus().pos;
	playerHeight_ = player.GetStatus().height;

	HitCollPlayer(player);

	priorityStateLeft_ = HitState::noHit;
	priorityStateRight_ = HitState::noHit;

	isTransit_ = player.GetStatus().isTransit;

	//�D��X�e�[�^�X�̎擾
	PriorityState(hitStateListLeft_, priorityStateLeft_);
	PriorityState(hitStateListRight_, priorityStateRight_);

	if (priorityStateLeft_ == HitState::bottom && priorityStateRight_ == HitState::noHit) {
		moveState_ = HitState::bottom;
	}
	else if (priorityStateLeft_ == HitState::noHit && priorityStateRight_ == HitState::bottom) {
		moveState_ = HitState::top;
	}
	else if (priorityStateLeft_ != priorityStateRight_) {
		moveState_ = HitState::noHit;
	}

	//�t���[���𓮂���
	MoveFrame(priorityStateLeft_, priorityStateRight_, moveDescent_, isStart_);

	model_->SetPos(pos_);

	hitStateListLeft_.clear();
	hitStateListRight_.clear();

	//isStart_ = false;
}

void Steelyard::Draw()
{
	model_->Draw();
}

void Steelyard::HitCollPlayer(Player& player)
{
	//�w��̃t���[���ԍ����擾����
	int leftFrameNo = MV1SearchFrame(model_->GetModelHandle(), "left");
	int RightFrameNo = MV1SearchFrame(model_->GetModelHandle(), "right");

	//�R���W�������̍X�V
	MV1RefreshCollInfo(model_->GetModelHandle(), leftFrameNo);
	MV1RefreshCollInfo(model_->GetModelHandle(), RightFrameNo);

	//�V���̍��E�ʂŏՓ˔�����s��
	hitDimLeft_ = MV1CollCheck_Capsule(model_->GetModelHandle(), leftFrameNo, VAdd(player.GetStatus().pos,VGet(0,10,0)), VAdd(player.GetStatus().pos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f);
	hitDimRight_ = MV1CollCheck_Capsule(model_->GetModelHandle(), RightFrameNo, VAdd(player.GetStatus().pos,VGet(0,10,0)), VAdd(player.GetStatus().pos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f);

	//���X�g�ɂ܂Ƃ߂�
	SummarizeList(hitDimLeft_, hitStateListLeft_,isStart_);
	SummarizeList(hitDimRight_, hitStateListRight_, isStart_);

	//�Փ˔���̏���
	MV1CollResultPolyDimTerminate(hitDimLeft_);
	MV1CollResultPolyDimTerminate(hitDimRight_);
}

void Steelyard::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{

	//�����^�ђ���������ȍ~�̏������s��Ȃ�
	if (isTransit_) {
		return;
	}

	//�w��̃t���[���ԍ����擾����
	int leftFrameNo = MV1SearchFrame(model_->GetModelHandle(), "left");
	int RightFrameNo = MV1SearchFrame(model_->GetModelHandle(), "right");

	//�R���W�������̍X�V
	MV1RefreshCollInfo(model_->GetModelHandle(), leftFrameNo);
	MV1RefreshCollInfo(model_->GetModelHandle(), RightFrameNo);

	VECTOR deadPersonPos = deadPerson->GetModelPointer()->GetPos();

	//�V���̍��E�ʂŏՓ˔�����s��
	hitDimLeft_ = MV1CollCheck_Capsule(model_->GetModelHandle(), leftFrameNo, VAdd(deadPersonPos, VGet(0.0f, 10.0f, 0.0f)), VAdd(deadPersonPos, VGet(0.0f, playerHeight_, 0.0f)), 20.0f);
	hitDimRight_ = MV1CollCheck_Capsule(model_->GetModelHandle(), RightFrameNo, VAdd(deadPersonPos, VGet(0.0f, 10.0f, 0.0f)), VAdd(deadPersonPos, VGet(0.0f, playerHeight_ + 2.0f, 0.0f)), 20.0f);
	
	//���X�g�ɂ܂Ƃ߂�
	SummarizeList(hitDimLeft_, hitStateListLeft_, isStart_);
	SummarizeList(hitDimRight_, hitStateListRight_, isStart_);

	//�Փ˔���̏���
	MV1CollResultPolyDimTerminate(hitDimLeft_);
	MV1CollResultPolyDimTerminate(hitDimRight_);
}

void Steelyard::SummarizeList(MV1_COLL_RESULT_POLY_DIM hitDim, std::list<HitState>& list, bool& isStart)
{

	//�@�������ɏ�ɏ���Ă��邩
	//������Ԃ����Ă��邩
	//�������Ă��Ȃ��̂����擾����
	if (hitDim.HitNum > 0) {
		if (hitDim.Dim->Normal.y > 0.99999f) {
			list.push_back(HitState::bottom);
			isStart = true;
		}
		else if (hitDim.Dim->Normal.y < -0.99999f) {
			list.push_back(HitState::top);
			isStart = true;
		}
	}
	else {
		list.push_back(HitState::noHit);
	}
}

//�ŗD��ŏ������s��state���擾����
void Steelyard::PriorityState(std::list<HitState> list, HitState& hitState)
{
	//enum class�̕��я��ŗD�揇�ʂ�\���Ă���
	for (auto& state : list) {
		if (hitState > state) {
			hitState = state;
		}
	}
}

void Steelyard::MoveFrame(HitState LeftHitState, HitState RightHitState, float& moveDescent, bool isStart)
{

	//�w��t���[���̔ԍ����擾����
	int leftFrameNo = MV1SearchFrame(model_->GetModelHandle(), "left");
	int rightFrameNo = MV1SearchFrame(model_->GetModelHandle(), "right");
	//���f������t���[���̍��W���擾����
	VECTOR leftFramePos = MV1GetFramePosition(model_->GetModelHandle(), leftFrameNo);
	VECTOR rightFramePos = MV1GetFramePosition(model_->GetModelHandle(), rightFrameNo);

	//���~����
	float descendingLimit = InitialPosition_.y - 50.0f;
	//�㏸����
	float upwardRevisionLimit = InitialPosition_.y + 50.0f;

	if (isStart) {
		//if (HitState::bottom == LeftHitState && HitState::noHit == RightHitState) {
		if (moveState_ == HitState::bottom) {
			if (leftFramePos.y >= descendingLimit) {
				moveDescent -= move_speed;
			}
		}
		//else if(HitState::noHit == LeftHitState && HitState::bottom == RightHitState) {
		else if(moveState_ == HitState::top) {
			if (leftFramePos.y <= upwardRevisionLimit) {
				moveDescent += move_speed;
			}
		}
	}

	MATRIX mtx = {};

	//��
	mtx = MGetTranslate(VGet(0, moveDescent, 0));
	MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), leftFrameNo, mtx);
	//�E
	mtx = MGetTranslate(VGet(0, -moveDescent, 0));
	MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), rightFrameNo, mtx);
}
