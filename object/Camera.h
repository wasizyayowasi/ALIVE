#pragma once
#include <DxLib.h>

class Camera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�������W</param>
	/// <param name="viewPos">����ꏊ</param>
	Camera(VECTOR pos,VECTOR viewPos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Camera();

	/// <summary>
	/// �J�����̏�����
	/// </summary>
	/// <param name="targetPos">�J����������ʒu</param>
	void Init(VECTOR targetPos);

	/// <summary>
	/// �J�����̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="playerHeight">�v���C���[�̐g��</param>
	void Update(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �v���C���[��ǐՂ���J�����̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void TrackingCameraUpdate(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// ��_�J����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void FixedPointCamera(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �J�����̒����_����炷
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="playerHeight">�v���C���[�̐g��</param>
	void ChangeOfFocus(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �ڕW�̃|�W�V�����܂ňړ�����
	/// </summary>
	void EasingMoveCamera();

	/// <summary>
	/// �J�����̃|�W�V�������擾����
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	VECTOR GetPos() { return pos_; }

	/// <summary>
	/// �J����������ʒu���擾����
	/// </summary>
	/// <returns>�J����������ʒu</returns>
	VECTOR GetTarget() { return cameraViewingPos_; }

	/// <summary>
	/// �J�������ړ������擾����
	/// </summary>
	/// <returns>true : �ړ����@ false : �ړ����ĂȂ�</returns>
	bool GetMoving() { return isMoving_; }

	/// <summary>
	/// �J�������ڕW�Ƃ�����W�ƌ���ʒu��ݒ肷��
	/// </summary>
	/// <param name="targetPos">�ڕW�ʒu</param>
	/// <param name="targetViewPos">����ʒu</param>
	/// <param name="upVec">�J�����̏�����x�N�g��</param>
	void SetCameraTargetPosAndView(VECTOR targetPos, VECTOR targetViewPos, VECTOR upVec);

	/// <summary>
	/// �o�ߎ��Ԃ�0�ɖ߂�
	/// </summary>
	void LesetElapsedTime() { elapsedTime_ = 0.0f; }

	void DebugDraw();

private:

	/// <summary>
	/// �v���C���[��X���W��ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	float TrackingPosX(VECTOR playerPos);

	/// <summary>
	/// �v���C���[��Y���W��ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	float TrackingPosY(VECTOR playerPos, float playerHeight);

private:

	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float moveVecX = 0.0f;

	float elapsedTime_ = 0.0f;

	bool isMoving_ = false;

	DINPUT_JOYSTATE input_ = {};

	VECTOR pos_ = {};							//�J�����̃|�W�V����
	VECTOR upVec_ = {0,1,0};					//�J�����̌���ڕW�ʒu
	VECTOR cameraViewingPos_ = {};				//�J����������ʒu

	VECTOR cameraTargetPos_ = {};				//�J�����̖ڕW�ʒu
	VECTOR targetUpVec_ = {};					//�J�����̖ڕW�����
	VECTOR cameraTargetViewPos_ = {};			//�J�����̌���ڕW�ʒu

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

