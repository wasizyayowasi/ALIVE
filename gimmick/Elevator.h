#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class Elevator : public GimmickBase
{
public:
	//コンストラクタ
	Elevator(const char* const filename, LoadObjectInfo objInfo);
	Elevator(int handle, LoadObjectInfo objInfo);
	virtual ~Elevator();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

private:

	float upVec_ = 0.0f;

	std::shared_ptr<ManualCrank> crank_;

};

