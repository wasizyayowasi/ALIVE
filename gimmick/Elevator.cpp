#include "Elevator.h"
#include "ManualCrank.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../object/Player.h"

namespace {
	constexpr float ascent_limit = 200.0f;
}

Elevator::Elevator(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
}

Elevator::Elevator(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	initPos_ = objInfo.pos;

	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo("Crank");
	crank_ = std::make_shared<ManualCrank>(info);
	upVec_ = ascent_limit / crank_->GetMaxRotZ();
}

Elevator::~Elevator()
{
}

void Elevator::Update(Player& player, const InputState& input)
{

	if (crank_->HitCollPlayer(player)) {
		player.SetGimmickModelPointer(crank_);
	}

	pos_.y = crank_->GetRotZ()* upVec_ + initPos_.y;

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
