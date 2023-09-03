#pragma once
#include "DxLib.h"

class Camera
{
public:

	Camera();
	virtual ~Camera();

	void trackingCameraUpdate(VECTOR playerPos);
	void fixedPointCamera(VECTOR playerPos);

private:

	float fixedPointCameraDestinationPosX = 0.0f;		//��_�J�����̈ړ���ō��WX��ۊǂ���ϐ�

	VECTOR cameraPos_ = {0, 300, -600};					//�J�����̃|�W�V����

};

