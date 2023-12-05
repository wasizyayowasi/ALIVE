#pragma once
#include "GimmickBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Switch;
class Model;

class Elevator : public GimmickBase
{
public:
	//�R���X�g���N�^
	Elevator(int handle, LoadObjectInfo objInfo);
	virtual ~Elevator();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	void PlayerTracking(VECTOR playerPos);
	void TargetPosition();

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();
private:

	float moveVecY_ = 0.0f;							//Y���̈ړ����F�N�g��

	bool isDeparture_ = false;						//�G���x�[�^�[���o�����Ă��邩�ǂ���

	VECTOR targetPos_ = {};							//���݌������Ă���|�W�V����

	std::vector<VECTOR> destinationPos_;			//�ړI�n�̃|�W�V����

	std::shared_ptr<Switch> switch_;
};

