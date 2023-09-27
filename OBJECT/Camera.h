#pragma once
#include "DxLib.h"

class InputState;

class Camera
{
public:

	Camera();
	virtual ~Camera();

	void trackingCameraUpdate(const InputState& input,VECTOR playerPos);
	void fixedPointCamera(VECTOR playerPos);

	void changeOfFocus(const InputState& input);							//�J�����̃^�[�Q�b�g�̈ʒu����炷
	void tracking(VECTOR playerPos);

	VECTOR getPos() { return cameraPos_; }
	VECTOR getTarget() { return cameraTarget_; }

private:

	int i = 0;

	float threshold;
	float tempRoom[3];
	float fixedPointCameraDestinationPosX = 0.0f;		//��_�J�����̈ړ���ō��WX��ۊǂ���ϐ�

	VECTOR cameraPos_ = {0, 270, -800};					//�J�����̃|�W�V����
	VECTOR cameraTarget_ = {0, 0, 0};					//�J�����̃^�[�Q�b�g

};

