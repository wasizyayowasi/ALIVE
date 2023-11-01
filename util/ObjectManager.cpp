#include "ObjectManager.h"
#include "object/ObjectBase.h"

#include "object/Player.h"
#include "../object/DeadPerson.h"
#include "object/tempEnemy.h"
#include "object/OrnamentBase.h"
#include "object/CarryObjectBase.h"
#include "../gimmick/TransparentObject.h"
#include "gimmick/steelyard.h"

namespace {
	const char* const player_Filename = "data/player/player16.mv1";

	const char* const temp_fieldpath = "data/model/tempFiled4.mv1";
	const char* const temp_stairs = "data/model/stairs.mv1";
	const char* const box_filename = "data/model/box.mv1";
	const char* const bigPillar_filename = "data/level0_model/bigPillar.mv1";
	const char* const switch_filename = "data/model/switch.mv1";
	const char* const steelyard_filename = "data/model/steelyard.mv1";
	const char* const transparent_obj_filename = "data/model/trans.mv1";
}

ObjectManager::ObjectManager()
{
	playerHandle_ = MV1LoadModel(player_Filename);
	fieldHandle_ = MV1LoadModel(temp_fieldpath);
	boxHandle_ = MV1LoadModel(box_filename);
	switchHandle_ = MV1LoadModel(switch_filename);
	steelyardHandle_ = MV1LoadModel(steelyard_filename);
	transObjHandle_ = MV1LoadModel(transparent_obj_filename);
}

ObjectManager::~ObjectManager()
{
	MV1DeleteModel(playerHandle_);
	MV1DeleteModel(fieldHandle_);
	MV1DeleteModel(boxHandle_);
	MV1DeleteModel(switchHandle_);
	MV1DeleteModel(steelyardHandle_);
	MV1DeleteModel(transObjHandle_);
}

void ObjectManager::ObjectGenerator(ObjectBaseType baseType, ObjectType objType, LoadObjectInfo objInfo)
{
	//objectBaseType�����ɃC���X�^���X������N���X�����߂�
	switch (baseType) {

	//�����������enemy�ȊO�̃L�����N�^�[�𐶐�
	case ObjectBaseType::characterBase:
		CharacterGenerator(objType, objInfo);
		break;

	//enemy�𐶐�
	case ObjectBaseType::enemyBase:
		EnemyGenerator(objType, objInfo);
		break;

	//�u���𐶐�
	case ObjectBaseType::ornamentBase:
		OrnamentGenerator(objType, objInfo);
		break;
	
	//�^�ׂ�u������
	case ObjectBaseType::carryBase:
		CarryObjectGenerator(objType, objInfo);
		break;

	//�M�~�b�N�𐶐�
	case ObjectBaseType::gimmickBase:
		GimmickObjectGenerator(objType, objInfo);
		break;
	case ObjectBaseType::temp:
		OrnamentGenerator(objType, objInfo);
		break;
	}

}

void ObjectManager::DeadPersonGenerator(int handle, LoadObjectInfo objInfo, int animNo)
{
	objects_[ObjectType::deadPerson].push_back(std::make_shared<DeadPerson>(handle, objInfo, animNo));

	if(objects_[ObjectType::deadPerson].size() < 4) return;

	objects_[ObjectType::deadPerson].remove(objects_[ObjectType::deadPerson].front());

}

//�X�V
void ObjectManager::Update(Player& player)
{
	//objects_�̊e�v�f��isEnable���擾���A�����ɂȂ��Ă���ΊY���R���e�i�̍폜
	std::erase_if(objects_, [](const auto& obj) {return !obj.second.front()->IsEnabled(); });

	//���̂̃|�C���^�[�����W����
	std::list<std::shared_ptr<ObjectBase>> deadPerson = {};
	for (auto obj : objects_) {
		for (auto objSecond : obj.second) {
			if (obj.first == ObjectType::deadPerson) {
				deadPerson.push_back(objSecond);
			}
		}
	}

	//���̂Ƃ��̑��̃I�u�W�F�N�g�̏Փ˔�����s��
	for (auto& obj : objects_) {
		for (auto& objSecond : obj.second) {
			if (obj.first != ObjectType::deadPerson) {
				for (auto& dead : deadPerson) {
					objSecond->HitColl(dead);
				}
			}
		}
	}

	//�X�V
	for (auto obj : objects_) {
		for (auto objSecond : obj.second) {
			objSecond->Update(player);
		}
	}

}

//�`��
void ObjectManager::Draw()
{
	for (auto& objs : objects_) {
		for (auto& obj : objs.second) {
			obj->Draw();
		}
	}
}

std::list<std::shared_ptr<Model>> ObjectManager::GetAllCheckCollModel()
{

	std::list<std::shared_ptr<Model>> checkCollList;

	for (auto& obj : objects_) {
		for (auto& model : obj.second) {
			if (model->IsCollCheck()) {
				checkCollList.push_back(model->GetModelPointer());
			}
		}
	}

	return checkCollList;
}

std::list<std::shared_ptr<Model>> ObjectManager::GetSpecificModel(ObjectType type)
{

	std::list<std::shared_ptr<Model>> specificList;

	//�����Ŏw�肳�ꂽ�I�u�W�F�N�g�̃��f���|�C���^����L�Ő錾����
	//�z��ɑ������
	for (auto& obj : objects_) {
		//�����ƈႤ�^�C�v�������ꍇcontinue
		if (obj.first != type) {
			continue;
		}
		//�w�肳�ꂽ�^�C�v�������ꍇ
		//���̃I�u�W�F�N�g�̃��X�g���烂�f���|�C���^���擾��
		//specificList�ɑ������
		for (auto& objSecond : obj.second) {
			specificList.push_back(objSecond->GetModelPointer());
		}
	}

	//���X�g��Ԃ�
	return specificList;
}

//�L�����N�^�[�����@
void ObjectManager::CharacterGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType)
	{
	case ObjectType::player:
		//objects_[objType].push_front(std::make_shared<Player>(filename));
		break;
	case ObjectType::deadPerson:
		//objects_[objType].push_front(std::make_shared<DeadPerson>(filename,10));
		break;
	}
	
}

//�G�����@
void ObjectManager::EnemyGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType) {
	case ObjectType::enemy:
		objects_[objType].push_front(std::make_shared<tempEnemy>(playerHandle_, objInfo));
		break;
	}
}

//�u�������@
void ObjectManager::OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType) {
	case ObjectType::field:
		objects_[objType].push_front(std::make_shared<OrnamentBase>(fieldHandle_, objInfo));
		break;
	}
}

//�^�ׂ�u�������@
void ObjectManager::CarryObjectGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType) {
	case ObjectType::carry:
		objects_[objType].push_front(std::make_shared <CarryObjectBase>(boxHandle_, objInfo));
		break;
	}
}

void ObjectManager::GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType) {
	case ObjectType::trans:
		objects_[objType].push_front(std::make_shared<TransparentObject>(transObjHandle_, objInfo));
		break;
	case ObjectType::gimmickSteelyard:
		objects_[objType].push_front(std::make_shared<Steelyard>(steelyardHandle_, objInfo));
	}
}
