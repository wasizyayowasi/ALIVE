#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class Elevator : public GimmickBase
{
public:
	//�R���X�g���N�^
	Elevator(const char* const filename, LoadObjectInfo objInfo);
	Elevator(int handle, LoadObjectInfo objInfo);
	virtual ~Elevator();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();

private:

	float upVec_ = 0.0f;

	std::shared_ptr<ManualCrank> crank_;

};

