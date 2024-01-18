#include "Model.h"
#include <cassert>

namespace {
	const char* const collision_frame_name = "Collision";
}

//�t�@�C���p�X�̃R���X�g���N�^
Model::Model(const char* fileName)
{
	modelHandle_ = MV1LoadModel(fileName);
	assert(modelHandle_ != -1);

	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);
}

//duplicate���f���̃R���X�g���N�^
Model::Model(int orgModel)
{
	modelHandle_ = MV1DuplicateModel(orgModel);
	assert(modelHandle_ != -1);

	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);
}

//�f�X�g���N�^
Model::~Model()
{
	MV1DeleteModel(modelHandle_);
}

//collision�t���[����collision�Ƃ��Ďg��
void Model::SetUseCollision(bool isUse, bool isNeedUpdate)
{
	assert(isUse | !isNeedUpdate);

	if (isUseCollision_ != isUse) {
		if (isUse) {
			colFrameIndex_ = MV1SearchFrame(modelHandle_, collision_frame_name);
			if (colFrameIndex_ < 0) {
				colFrameIndex_ = -1;
			}
			MV1SetupCollInfo(modelHandle_, colFrameIndex_, 8, 8, 8);
		}
		else {
			MV1TerminateCollInfo(modelHandle_, -1);
		}
	}

	isUseCollision_ = isUse;
	isUpdateCollsion_ = isNeedUpdate;
}

//�X�V
void Model::Update()
{
	UpdateAnim(animPrev_);
	UpdateAnim(animNext_);

	animChangeFrame_++;
	if (animChangeFrame_ > animChangeFrameTotal_) {
		animChangeFrame_ = animChangeFrameTotal_;
	}

	UpdateAnimBlendRate();

	if (isUpdateCollsion_ && isUpdateCollsion_) {
		MV1RefreshCollInfo(modelHandle_, colFrameIndex_);
	}
}

//�`��
void Model::Draw()
{
	MV1DrawModel(modelHandle_);
}

//�|�W�V�����ݒ�
void Model::SetPos(VECTOR pos)
{
	pos_ = pos;
	MV1SetPosition(modelHandle_, pos);
}

//�p�x�ݒ�
void Model::SetRot(VECTOR rot)
{
	rot_ = rot;
	MV1SetRotationXYZ(modelHandle_, rot);
}

//�g�k���ݒ�
void Model::SetScale(VECTOR scale)
{
	MV1SetScale(modelHandle_,scale);
}

//collision�t���[���ݒ�
void Model::SetCollFrame(const char* collFrameName)
{
	colFrameIndex_ = MV1SearchFrame(modelHandle_, collFrameName);
	if (colFrameIndex_ < 0) {
		colFrameIndex_ = -1;
	}
	MV1SetupCollInfo(modelHandle_, colFrameIndex_, 8, 8, 8);
}

//�A�j���[�V�����ݒ�
void Model::SetAnimation(int animNo, bool isLoop, bool IsForceChange)
{
	if (!IsForceChange) {
		if (animNext_.animNo == animNo)return;
	}

	if (animPrev_.attachNo != -1) {
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		ClearAnimData(animPrev_);
	}
	if (animNext_.attachNo != -1) {
		MV1DetachAnim(modelHandle_, animNext_.attachNo);
		ClearAnimData(animNext_);
	}

	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNext_.animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = 1;
	animChangeFrame_ = 1;
}

//�A�j���[�V�����ύX
void Model::ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame)
{
	if (!isForceChange)
	{
		if (animNext_.animNo == animNo)	return;
	}

	if (animPrev_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		ClearAnimData(animPrev_);
	}
	animPrev_ = animNext_;

	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = changeFrame;
	animChangeFrame_ = 0;

	UpdateAnimBlendRate();
}

//�A�j���[�V�����̏I�����擾
bool Model::IsAnimEnd()
{
	if (animNext_.isLoop) return false;

	float time = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (time >= animNext_.totalTime) return true;
	return false;
}

//�A�j���[�V�����̏I����ݒ�
void Model::SetAnimEndFrame(int animNo)
{
	SetAnimation(animNo, false, true);
	MV1SetAttachAnimTime(modelHandle_, animNext_.attachNo, animNext_.totalTime);
}

//����t���[���̍��W���擾
VECTOR Model::GetFrameLocalPosition(const char* frameName)
{
	int frameNo = MV1SearchFrame(modelHandle_, frameName);
	auto name = MV1GetFrameName(modelHandle_, frameNo);
	VECTOR localPos = MV1GetFramePosition(modelHandle_, frameNo);

	return localPos;
}

void Model::SetAnimationFrame(float value)
{
	MV1SetAttachAnimTime(modelHandle_, animNext_.attachNo, value);
}

bool Model::GetSpecifiedAnimTime(int specifiedTime)
{

	float currentAnimTime = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (currentAnimTime == specifiedTime) {
		return true;
	}

	return false;
}

//�A�j���[�V�������̏�����
void Model::ClearAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

//�A�j���[�V�����̍X�V
void Model::UpdateAnim(AnimData anim, float dt)
{
	if (anim.attachNo == -1) return;

	float time = MV1GetAttachAnimTime(modelHandle_, anim.attachNo);
	time += dt;
	if (time > anim.totalTime)
	{
		if (anim.isLoop)
		{
			time -= anim.totalTime;
		}
		else
		{
			time = anim.totalTime;
		}
	}
	MV1SetAttachAnimTime(modelHandle_, anim.attachNo, time);
}

//�A�j���[�V�����̃u�����h
void Model::UpdateAnimBlendRate()
{
	float rate = static_cast<float> (animChangeFrame_) / static_cast<float>(animChangeFrameTotal_);
	if (rate > 1.0f)	rate = 1.0f;
	MV1SetAttachAnimBlendRate(modelHandle_, animPrev_.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(modelHandle_, animNext_.attachNo, rate);
}
