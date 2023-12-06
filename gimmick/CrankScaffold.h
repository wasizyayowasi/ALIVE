#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold : public GimmickBase
{
public:
	//�R���X�g���N�^
	CrankScaffold(int handle, LoadObjectInfo objInfo);
	virtual ~CrankScaffold();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();


private:

	float upVec_ = 0.0f;

	VECTOR initPos_ = {};


	std::shared_ptr<ManualCrank> crank_;

};

