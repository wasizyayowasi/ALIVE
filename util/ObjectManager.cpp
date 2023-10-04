#include "ObjectManager.h"
#include "object/ObjectBase.h"

#include "object/Player.h"
#include "../object/DeadPerson.h"
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
	//objectBaseTypeを元にインスタンス化するクラスを決める
	switch (baseType) {

	//自我を持ったenemy以外のキャラクターを生成
	case ObjectBaseType::characterBase:
		characterGenerator(objType, filename);
		break;

	//enemyを生成
	case ObjectBaseType::enemyBase:
		enemyGenerator(objType, filename);
		break;

	//置物を生成
	case ObjectBaseType::ornamentBase:
		ornamentGenerator(objType, filename);
		break;
	
	//運べる置物生成
	case ObjectBaseType::carryBase:
		carryObjectGenerator(objType, filename);
		break;

	//ギミックを生成
	case ObjectBaseType::gimmickBase:
		gimmickObjectGenerator(objType, filename);
		break;
	}

}

void ObjectManager::deadPersonGenerator(ObjectType objType, int handle, VECTOR pos, VECTOR rot, int animNo)
{
	objects_[objType].push_back(std::make_shared<DeadPerson>(handle, pos, rot, animNo));

	if(objects_[objType].size() < 4) return;

	objects_[objType].remove(objects_[objType].front());

}

//更新
void ObjectManager::update()
{
	//衝突判定を行うのモデルリストを削除する
	checkCollList_.erase(checkCollList_.begin(), checkCollList_.end());

	//objects_の各要素のisEnableを取得し、無効になっていれば該当コンテナの削除
	std::erase_if(objects_, [](const auto& obj) {return !obj.second.front()->isEnabled(); });

	//更新
	for (auto& obj : objects_) {
		for (auto& objSecond : obj.second) {
			objSecond->update();
		}
	}
}

//描画
void ObjectManager::draw()
{
	for (auto& objs : objects_) {
		for (auto& obj : objs.second) {
			obj->draw();
		}
	}
}

std::list<std::shared_ptr<Model>> ObjectManager::getCheckCollModel()
{
	for (auto& obj : objects_) {
		if (obj.first == ObjectType::player || obj.first == ObjectType::enemy) continue;
		for (auto& model : obj.second) {
			checkCollList_.push_back(model->getModelPointer());
		}
	}

	return checkCollList_;
}

//キャラクター生成機
void ObjectManager::characterGenerator(ObjectType objType, const char* const filename)
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

//敵生成機
void ObjectManager::enemyGenerator(ObjectType objType, const char* const filename)
{
	switch (objType) {
	case ObjectType::enemy:
		objects_[objType].push_front(std::make_shared<tempEnemy>(filename));
		break;
	}
}

//置物生成機
void ObjectManager::ornamentGenerator(ObjectType objType, const char* const filename)
{
	switch (objType) {
	case ObjectType::field:
		objects_[objType].push_front(std::make_shared<OrnamentBase>(filename));
		break;
	}
}

//運べる置物生成機
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
