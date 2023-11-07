#include "Elevator.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../gimmick/ManualCrank.h"
#include "../object/Player.h"

namespace {
	constexpr float ascent_limit = 200.0f;
}

Elevator::Elevator(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
}

Elevator::Elevator(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	crank_ = std::make_shared<ManualCrank>();
	upVec_ = ascent_limit / crank_->GetMaxRotZ();
}

Elevator::~Elevator()
{
}

void Elevator::Update(Player& player, const InputState& input)
{
//	if (crank_->HitCollPlayer(player)) {
//		if (input.IsTriggered(InputType::carry)) {
			crank_->Update(input);
//		}
//	}

	pos_.y = crank_->GetRotZ()* upVec_;

	model_->SetPos(pos_);

}
void Elevator::Draw()
{
	model_->Draw();
	crank_->Draw();
}

std::shared_ptr<Model> Elevator::AddCollModel()
{
	return crank_->GetModelPointer();
}
