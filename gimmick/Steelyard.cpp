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
}

Steelyard::Steelyard(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//��
	InitialPosition_ = pos_;
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

	//�t���[���𓮂���
	MoveFrame(priorityStateRight_, "right", moveDescentRight_, isStartRight_);
	MoveFrame(priorityStateLeft_, "left", moveDescentLeft_, isStartLeft_);

	model_->SetPos(pos_);

	hitStateListLeft_.clear();
	hitStateListRight_.clear();

	isStartRight_ = false;
	isStartLeft_ = false;
}

void Steelyard::Draw()
{
	model_->Draw();
	DrawSphere3D(pos_, 50, 32, 0xff0000, 0xff0000, true);
	DrawFormatString(0, 128, 0x448844, "%.2f", moveDescentLeft_);
	DrawFormatString(0, 144, 0x448844, "%.2f", moveDescentRight_);
	DrawFormatString(0, 160, 0x448844, "%.2f , %.2f , %.2f", framePos_.x, framePos_.y, framePos_.z);
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
	SummarizeList(hitDimLeft_, hitStateListLeft_,isStartLeft_);
	SummarizeList(hitDimRight_, hitStateListRight_,isStartRight_);

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

	//�V���̍��E�ʂŏՓ˔�����s��
	hitDimLeft_ = MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), leftFrameNo, pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f);
	hitDimRight_ = MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), RightFrameNo, pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f);

	//���X�g�ɂ܂Ƃ߂�
	SummarizeList(hitDimLeft_, hitStateListLeft_, isStartLeft_);
	SummarizeList(hitDimRight_, hitStateListRight_, isStartRight_);

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

void Steelyard::MoveFrame(HitState hitState, std::string string, float& moveDescent, bool isStart)
{

	//�w��t���[���̔ԍ����擾����
	int leftFrameNo = MV1SearchFrame(model_->GetModelHandle(), "left");
	int rightFrameNo = MV1SearchFrame(model_->GetModelHandle(), "right");
	//���f������t���[���̍��W���擾����
	VECTOR leftFramePos = MV1GetFramePosition(model_->GetModelHandle(), leftFrameNo);
	VECTOR rightFramePos = MV1GetFramePosition(model_->GetModelHandle(), rightFrameNo);

	float descendingLimit = InitialPosition_.y - 50.0f;
	float upwardRevisionLimit = InitialPosition_.y + 50.0f;

	if (isStart) {
		if (leftFramePos.y >= descendingLimit) {
			moveDescent -= move_speed;
		}
		else if (leftFramePos.y <= upwardRevisionLimit) {
			moveDescent += move_speed;
		}
	}

	MATRIX mtx = {};

	//�D��X�e�[�^�X��bottom��������
	if (hitState == HitState::bottom) {
		//��
		mtx = MGetTranslate(VGet(0, moveDescent, 0));
		MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), leftFrameNo, mtx);
		//�E
		mtx = MGetTranslate(VGet(0, -moveDescent, 0));
		MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), rightFrameNo, mtx);
	}
}
