#include "Steelyard.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
	constexpr float move_speed = 2.0f;
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
	HitCollPlayer(player);

	priorityStateLeft_ = HitState::noHit;
	priorityStateRight_ = HitState::noHit;

	isTransit_ = player.GetStatus().isTransit;

	//�D��X�e�[�^�X�̎擾
	PriorityState(hitStateListLeft_, priorityStateLeft_);
	PriorityState(hitStateListRight_, priorityStateRight_);

	//�t���[���𓮂���
	MoveFrame(priorityStateLeft_, "left");
	MoveFrame(priorityStateRight_, "right");

	model_->SetPos(pos_);

	hitStateListLeft_.clear();

	isStart_ = false;
}

void Steelyard::Draw()
{
	model_->Draw();
	DrawSphere3D(pos_, 50, 32, 0xff0000, 0xff0000, true);
	DrawFormatString(0, 128, 0x448844, "%.2f", moveDescent_);
	DrawFormatString(0, 144, 0x448844, "%.2f , %.2f , %.2f", framePos_.x, framePos_.y, framePos_.z);
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
	hitDimLeft_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), leftFrameNo, VAdd(player.GetStatus().pos,VGet(0,10,0)), VAdd(player.GetStatus().pos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f));
	hitDimRight_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), RightFrameNo, VAdd(player.GetStatus().pos,VGet(0,10,0)), VAdd(player.GetStatus().pos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f));

	//���X�g�ɂ܂Ƃ߂�
	SummarizeList(hitDimLeft_, hitStateListLeft_);
	SummarizeList(hitDimRight_, hitStateListRight_);

	//�Փ˔���̏���
	for (auto& hit : hitDimLeft_) {
		MV1CollResultPolyDimTerminate(hit);
	}
	for (auto& hit : hitDimRight_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDimLeft_.clear();
	hitDimRight_.clear();
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
	hitDimLeft_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), leftFrameNo, pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));
	hitDimRight_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), RightFrameNo, pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));

	//���X�g�ɂ܂Ƃ߂�
	SummarizeList(hitDimLeft_, hitStateListLeft_);
	SummarizeList(hitDimRight_, hitStateListRight_);

	//�Փ˔���̏���
	for (auto& hit : hitDimLeft_) {
		MV1CollResultPolyDimTerminate(hit);
	}
	for (auto& hit : hitDimRight_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDimLeft_.clear();
	hitDimRight_.clear();

}

void Steelyard::SummarizeList(std::list<MV1_COLL_RESULT_POLY_DIM> hitDim, std::list<HitState>& list)
{
	//�@�������ɏ�ɏ���Ă��邩
	//������Ԃ����Ă��邩
	//�������Ă��Ȃ��̂����擾����
	for (auto& dim : hitDim) {
		if (dim.HitNum > 0) {
			if (dim.Dim->Normal.y > 0.99999f) {
				list.push_back(HitState::bottom);
				isStart_ = true;
			}
			else if (dim.Dim->Normal.y < -0.99999f) {
				list.push_back(HitState::top);
				isStart_ = true;
			}
		}
		else {
			list.push_back(HitState::noHit);
		}
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

void Steelyard::MoveFrame(HitState hitState, std::string string)
{
	//�w��t���[���̔ԍ����擾����
	int frameNo = MV1SearchFrame(model_->GetModelHandle(), string.c_str());
	//���f������t���[���̍��W���擾����
	VECTOR framePos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
	framePos_ = framePos;

	//�����l���牺�~�����������������W���擾
	VECTOR descentLimit = VSub(InitialPosition_, VGet(0, 50, 0));

	if (isStart_) {
		moveDescent_ -= 0.01f;
	}
	else {
		moveDescent_ = 0.0f;
	}

	//�D��X�e�[�^�X��bottom��������
	if (hitState == HitState::bottom) {
		if (framePos.y > descentLimit.y) {
			MATRIX mtx = MGetTranslate(VGet(0, moveDescent_, 0));
			MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), frameNo, mtx);
		}
	}
	else if (hitState == HitState::noHit) {
		VECTOR distance = VSub(framePos, InitialPosition_);
		float size = VSize(distance);
		if (size > 0) {
			if (framePos.y > InitialPosition_.y) {
				move_ -= 1.00f;
			}
		}
		else {
			if (framePos.y < InitialPosition_.y) {
				move_ += 0.01f;
			}
		}
		MATRIX mtx = MGetTranslate(VGet(0, move_, 0));
		MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), frameNo, mtx);
	}


	/*{
		if (isStart_) {
			moveDescent_ += 0.01f;
			if (moveDescent_ > 5.0f) {
				isStart_ = false;
			}
		}
		else {
			moveDescent_ -= 0.01f;
			if (moveDescent_ < -5.0f) {
				isStart_ = true;
			}
		}

		MATRIX mtx = MGetTranslate(VGet(0, moveDescent_, 0));
		MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), frameNo, mtx);
	}*/

}
