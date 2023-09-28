#pragma once
#include "DxLib.h"

class InputState;

class Camera
{
public:

	Camera();
	virtual ~Camera();

	/// <summary>
	/// �v���C���[��ǐՂ���J�����̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void trackingCameraUpdate(const InputState& input,VECTOR playerPos);

	/// <summary>
	/// ��_�J����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void fixedPointCamera(VECTOR playerPos);

	/// <summary>
	/// �J�����̒����_����炷
	/// </summary>
	/// <param name="input">���͏��</param>
	void changeOfFocus(const InputState& input);

	/// <summary>
	/// �v���C���[�̒ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
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

