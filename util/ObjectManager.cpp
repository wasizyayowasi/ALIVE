#include "ObjectManager.h"
#include "object/ObjectBase.h"

#include "object/Player.h"
#include "../object/DeadPerson.h"
#include "object/tempEnemy.h"
#include "object/OrnamentBase.h"
#include "object/CarryObjectBase.h"
#include "gimmick/Switch.h"
#include "gimmick/steelyard.h"

namespace {
	const char* const player_Filename = "data/player/player14.mv1";

	const char* const temp_fieldpath = "data/model/tempFiled4.mv1";
	const char* const temp_stairs = "data/model/stairs.mv1";
	const char* const box_filename = "data/model/box.mv1";
	const char* const bigPillar_filename = "data/level0_model/bigPillar.mv1";
	const char* const switch_filename = "data/model/switch.mv1";
	const char* const steelyard_filename = "data/model/steelyard.mv1";
}

ObjectManager::ObjectManager()
{
	playerHandle_ = MV1LoadModel(player_Filename);
	fieldHandle_ = MV1LoadModel(temp_fieldpath);
	boxHandle_ = MV1LoadModel(box_filename);
	switchHandle_ = MV1LoadModel(switch_filename);
	steelyardHandle_ = MV1LoadModel(steelyard_filename);
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::ObjectGenerator(ObjectBaseType baseType, ObjectType objType)
{
	//objectBaseType�����ɃC���X�^���X������N���X�����߂�
	switch (baseType) {

	//�����������enemy�ȊO�̃L�����N�^�[�𐶐�
	case ObjectBaseType::characterBase:
		CharacterGenerator(objType);
		break;

	//enemy�𐶐�
	case ObjectBaseType::enemyBase:
		EnemyGenerator(objType);
		break;

	//�u���𐶐�
	case ObjectBaseType::ornamentBase:
		OrnamentGenerator(objType);
		break;
	
	//�^�ׂ�u������
	case ObjectBaseType::carryBase:
		CarryObjectGenerator(objType);
		break;

	//�M�~�b�N�𐶐�
	case ObjectBaseType::gimmickBase:
		GimmickObjectGenerator(objType);
		break;
	}

}

void ObjectManager::DeadPersonGenerator(ObjectType objType, int handle, VECTOR pos, VECTOR rot, int animNo)
{
	objects_[objType].push_back(std::make_shared<DeadPerson>(handle, pos, rot, animNo));

	if(objects_[objType].size() < 4) return;

	objects_[objType].remove(objects_[objType].front());

}

//�X�V
void ObjectManager::Update(Player& player)
{
	//objects_�̊e�v�f��isEnable���擾���A�����ɂȂ��Ă���ΊY���R���e�i�̍폜
	std::erase_if(objects_, [](const auto& obj) {return !obj.second.front()->IsEnabled(); });

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
		if (obj.first == ObjectType::player || obj.first == ObjectType::enemy) continue;
		for (auto& model : obj.second) {
			checkCollList.push_back(model->GetModelPointer());
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
void ObjectManager::CharacterGenerator(ObjectType objType)
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
void ObjectManager::EnemyGenerator(ObjectType objType)
{
	switch (objType) {
	case ObjectType::enemy:
		objects_[objType].push_front(std::make_shared<tempEnemy>(playerHandle_));
		break;
	}
}

//�u�������@
void ObjectManager::OrnamentGenerator(ObjectType objType)
{
	switch (objType) {
	case ObjectType::field:
		objects_[objType].push_front(std::make_shared<OrnamentBase>(fieldHandle_));
		break;
	}
}

//�^�ׂ�u�������@
void ObjectManager::CarryObjectGenerator(ObjectType objType)
{
	switch (objType) {
	case ObjectType::carry:
		objects_[objType].push_front(std::make_shared <CarryObjectBase>(boxHandle_));
		break;
	}
}

void ObjectManager::GimmickObjectGenerator(ObjectType objType)
{
	switch (objType) {
	case ObjectType::gimmickSwitch:
		objects_[objType].push_front(std::make_shared<Switch>(switchHandle_));
		break;
	case ObjectType::gimmickSteelyard:
		objects_[objType].push_front(std::make_shared<Steelyard>(steelyardHandle_));
	}
}
