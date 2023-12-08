#pragma once
#include "GimmickBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Switch;
class Lever;
class Model;

enum class ElevatorAnimType {
	openIdle,
	closeIdle,
	open,
	close,
	max,
};

class Elevator : public GimmickBase
{
public:
	//�R���X�g���N�^
	Elevator(int handle, LoadObjectInfo objInfo);
	virtual ~Elevator();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	void Move();

	void TargetPosition();

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();
private:

	int departureTime_ = 0;

	float moveVecY_ = 0.0f;							//Y���̈ړ����F�N�g��

	bool isDeparture_ = false;						//�G���x�[�^�[���o�����Ă��邩�ǂ���

	VECTOR targetPos_ = {};							//���݌������Ă���|�W�V����

	std::shared_ptr<Switch> switch_;

	std::vector<std::shared_ptr<Lever>> levers_;
};

