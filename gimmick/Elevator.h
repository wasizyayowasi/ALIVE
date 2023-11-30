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

	void TargetPosition(float distanceY);

	//�Փ˔�����s�����f����ǉ�����
//	virtual std::shared_ptr<Model> AddCollModel();
private:

	float moveVecY = 0.0f;

	VECTOR targetPos = {};

	std::vector<std::shared_ptr<Switch>> switch_;
};

