#pragma once
#include <DxLib.h>

class InputState;

class Camera
{
public:

	Camera();
	virtual ~Camera();

	//�������p
	void Init();

	/// <summary>
	/// �v���C���[��ǐՂ���J�����̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void TrackingCameraUpdate(VECTOR playerPos);

	/// <summary>
	/// ��_�J����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void FixedPointCamera(VECTOR playerPos);

	/// <summary>
	/// �J�����̒����_����炷
	/// </summary>
	/// <param name="input">���͏��</param>
	void ChangeOfFocus(const InputState& input);

	/// <summary>
	/// �v���C���[�̒ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void Tracking(VECTOR playerPos);

	VECTOR GetPos() { return cameraPos_; }
	VECTOR GetTarget() { return cameraTarget_; }

	void DebugCamera(VECTOR playerPos);

	float TrackingPozZ(VECTOR playerPos);


	void tempDraW();

private:

	int i = 0;
	int cameraTargetPosZ = 0;

	DINPUT_JOYSTATE input_;

	float threshold;
	float tempRoom[3];
	float fixedPointCameraDestinationPosX = 0.0f;		//��_�J�����̈ړ���ō��WX��ۊǂ���ϐ�

	VECTOR cameraPos_;									//�J�����̃|�W�V����
	VECTOR cameraTarget_ = {0, 0, 0};					//�J�����̃^�[�Q�b�g

};

