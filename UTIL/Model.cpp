#include "Model.h"
#include <cassert>

namespace {
	const char* const collision_frame_name = "Collision";
}

Model::Model(const char* fileName)
{
	modelHandle_ = MV1LoadModel(fileName);
	assert(modelHandle_ != -1);

	clearAnimData(animPrev_);
	clearAnimData(animNext_);
}

Model::Model(int orgModel)
{
	assert(modelHandle_ != -1);
	modelHandle_ = MV1DuplicateModel(orgModel);

	clearAnimData(animPrev_);
	clearAnimData(animNext_);
}

Model::~Model()
{
	MV1DeleteModel(modelHandle_);
}

void Model::setUseCollision(bool isUse, bool isNeedUpdate)
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

void Model::update()
{
	updateAnim(animPrev_);
	updateAnim(animNext_);

	animChangeFrame_++;
	if (animChangeFrame_ > animChangeFrameTotal_) {
		animChangeFrame_ = animChangeFrameTotal_;
	}

	updateAnimBlendRate();

	if (isUpdateCollsion_ && isUpdateCollsion_) {
		MV1RefreshCollInfo(modelHandle_, colFrameIndex_);
	}
}

void Model::draw()
{
	MV1DrawModel(modelHandle_);
}

void Model::setPos(VECTOR pos)
{
	MV1SetPosition(modelHandle_, pos);
}

void Model::setRot(VECTOR rot)
{
	MV1SetRotationXYZ(modelHandle_, rot);
}

void Model::setScale(VECTOR scale)
{
	MV1SetScale(modelHandle_,scale);
}

void Model::setAnimation(int animNo, bool isLoop, bool IsForceChange)
{
	if (!IsForceChange) {
		if (animNext_.animNo == animNo)return;
	}

	if (animPrev_.attachNo != -1) {
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		clearAnimData(animPrev_);
	}
	if (animNext_.attachNo != -1) {
		MV1DetachAnim(modelHandle_, animNext_.attachNo);
		clearAnimData(animNext_);
	}

	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = 1;
	animChangeFrame_ = 1;
}

void Model::changeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame)
{
	if (!isForceChange)
	{
		if (animNext_.animNo == animNo)	return;
	}

	if (animPrev_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		clearAnimData(animPrev_);
	}
	animPrev_ = animNext_;

	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = changeFrame;
	animChangeFrame_ = 0;

	updateAnimBlendRate();
}

bool Model::isAnimEnd()
{
	if (animNext_.isLoop) return false;

	float time = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (time >= animNext_.totalTime) return true;
	return false;
}

void Model::clearAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

void Model::updateAnim(AnimData anim, float dt)
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

void Model::updateAnimBlendRate()
{
	float rate = static_cast<float> (animChangeFrame_) / static_cast<float>(animChangeFrameTotal_);
	if (rate > 1.0f)	rate = 1.0f;
	MV1SetAttachAnimBlendRate(modelHandle_, animPrev_.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(modelHandle_, animNext_.attachNo, rate);
}
