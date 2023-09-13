#pragma once
#include "DxLib.h"

class Model
{
public:
	Model(const char* fileName);
	Model(int orgModel);

	virtual ~Model();

	void setUseCollision(bool isUse, bool isNeedUpdate);

	void update();
	void draw();

	int getModelHandle() const { return modelHandle_; }
	int getColFrameIndex() const { return colFrameIndex_; }
	bool getEnable() const { return isEnable_; }
	VECTOR getPos() const { return pos_; }

	void setPos(VECTOR pos);
	void setRot(VECTOR rot);
	void setScale(VECTOR scale);

	void setAnimation(int animNo,bool isLoop,bool IsForceChange);

	void changeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame);

	bool isAnimEnd();

	void setAnimEndFrame(int animNo);

private:
	struct AnimData {
		int animNo;
		int attachNo;
		float totalTime;
		bool isLoop;
	};
private:
	void clearAnimData(AnimData& anim);

	void updateAnim(AnimData anim, float dt = 1.0f);

	void updateAnimBlendRate();
private:
	int modelHandle_ = -1;

	bool isUseCollision_ = false;

	bool isUpdateCollsion_ = false;

	bool isEnable_ = false;

	int colFrameIndex_ = 0;

	AnimData animPrev_;
	AnimData animNext_;

	int animChangeFrame_ = 0;
	int animChangeFrameTotal_ = 0;

	VECTOR pos_;

};

