#include "ObjectBase.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/InputState.h"

ObjectBase::ObjectBase(int handle, Material materialType, LoadObjectInfo objInfo)
{
	//���f���̐ݒ�
	model_ = std::make_shared<Model>(handle,materialType);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);

	//�|�W�V����
	pos_ = model_->GetPos();

	//�g�k��
	scale_ = objInfo.scale;

	//���݂��Ă���t���O
	isEnable_ = true;
}

ObjectBase::~ObjectBase()
{
}

void ObjectBase::Update(Player& player)
{
	model_->Update();
}

void ObjectBase::Draw()
{
	model_->Draw();
}

void ObjectBase::UpdateForCorpse(std::shared_ptr<ObjectBase> pointer)
{
}

std::shared_ptr<Model> ObjectBase::AddCollModel()
{
	return nullptr;
}