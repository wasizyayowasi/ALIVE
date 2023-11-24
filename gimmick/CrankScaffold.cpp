#include "CrankScaffold.h"
#include "ManualCrank.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../object/Player.h"

namespace {
	constexpr float ascent_limit = 200.0f;
}

CrankScaffold::CrankScaffold(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
}

CrankScaffold::CrankScaffold(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	initPos_ = objInfo.pos;

	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo("Crank");
	crank_ = std::make_shared<ManualCrank>(info);
	upVec_ = ascent_limit / crank_->GetMaxRotZ();
}

CrankScaffold::~CrankScaffold()
{
}

void CrankScaffold::Update(Player& player, const InputState& input)
{

	if (crank_->HitCollPlayer(player)) {
		player.SetGimmickModelPointer(crank_);
	}

	pos_.y = crank_->GetRotZ()* upVec_ + initPos_.y;

	model_->SetPos(pos_);

}
void CrankScaffold::Draw()
{
	model_->Draw();
	crank_->Draw();
}

std::shared_ptr<Model> CrankScaffold::AddCollModel()
{
	return crank_->GetModelPointer();
}
