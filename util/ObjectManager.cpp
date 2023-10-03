#include "ObjectManager.h"
#include "object/ObjectBase.h"

#include "object/Player.h"
#include "object/tempEnemy.h"
#include "object/OrnamentBase.h"
#include "object/CarryObjectBase.h"
#include "gimmick/Switch.h"
#include "gimmick/steelyard.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::objectGenerator(ObjectBaseType baseType, ObjectType objType, const char* const filename)
{
	//objectBaseType�����ɃC���X�^���X������N���X�����߂�
	switch (baseType) {

	//�����������enemy�ȊO�̃L�����N�^�[�𐶐�
	case ObjectBaseType::characterBase:
		characterGenerator(objType, filename);
		break;

	//enemy�𐶐�
	case ObjectBaseType::enemyBase:
		enemyGenerator(objType, filename);
		break;

	//�u���𐶐�
	case ObjectBaseType::ornamentBase:
		ornamentGenerator(objType, filename);
		break;
	
	//�^�ׂ�u������
	case ObjectBaseType::carryBase:
		carryObjectGenerator(objType, filename);
		break;

	//�M�~�b�N�𐶐�
	case ObjectBaseType::gimmickBase:
		gimmickObjectGenerator(objType, filename);
		break;
	}
}

//�X�V
void ObjectManager::update()
{
	//objects_�̊e�v�f��isEnable���擾���A�����ɂȂ��Ă���ΊY���R���e�i�̍폜
	std::erase_if(objects_, [](const auto& obj) {return !obj.second.front()->isEnabled(); });

	//�X�V
	for (auto& obj : objects_) {
		obj.second.front()->update();
	}
}

//�`��
void ObjectManager::draw()
{
	for (auto& obj : objects_) {
		obj.second.front()->draw();
	}
}

//�L�����N�^�[�����@
void ObjectManager::characterGenerator(ObjectType objType, const char* const filename)
{
	switch (objType)
	{
	case ObjectType::player:
		//objects_[objType].push_front(std::make_shared<Player>(filename));
		break;
	}
	
}

//�G�����@
void ObjectManager::enemyGenerator(ObjectType objType, const char* const filename)
{
	switch (objType) {
	case ObjectType::enemy:
		objects_[objType].push_front(std::make_shared<tempEnemy>(filename));
		break;
	}
}

//�u�������@
void ObjectManager::ornamentGenerator(ObjectType objType, const char* const filename)
{
	switch (objType) {
	case ObjectType::field:
		objects_[objType].push_front(std::make_shared<OrnamentBase>(filename));
		break;
	}
}

//�^�ׂ�u�������@
void ObjectManager::carryObjectGenerator(ObjectType objType, const char* const filename)
{
	switch (objType) {
	case ObjectType::carry:
		objects_[objType].push_front(std::make_shared <CarryObjectBase>(filename));
		break;
	}
}

void ObjectManager::gimmickObjectGenerator(ObjectType objType, const char* const filename)
{
	switch (objType) {
	case ObjectType::gimmickSwitch:
		objects_[objType].push_front(std::make_shared<Switch>(filename));
		break;
	case ObjectType::gimmickSteelyard:
		objects_[objType].push_front(std::make_shared<Steelyard>(filename));
	}
}
