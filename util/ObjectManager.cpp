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
	//objectBaseTypeを元にインスタンス化するクラスを決める
	switch (baseType) {

	//自我を持ったenemy以外のキャラクターを生成
	case ObjectBaseType::characterBase:
		CharacterGenerator(objType);
		break;

	//enemyを生成
	case ObjectBaseType::enemyBase:
		EnemyGenerator(objType);
		break;

	//置物を生成
	case ObjectBaseType::ornamentBase:
		OrnamentGenerator(objType);
		break;
	
	//運べる置物生成
	case ObjectBaseType::carryBase:
		CarryObjectGenerator(objType);
		break;

	//ギミックを生成
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

//更新
void ObjectManager::Update(Player& player)
{
	//objects_の各要素のisEnableを取得し、無効になっていれば該当コンテナの削除
	std::erase_if(objects_, [](const auto& obj) {return !obj.second.front()->IsEnabled(); });

	//更新
	for (auto obj : objects_) {
		for (auto objSecond : obj.second) {
			objSecond->Update(player);
		}
	}
}

//描画
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

	//引数で指定されたオブジェクトのモデルポインタを上記で宣言した
	//配列に代入する
	for (auto& obj : objects_) {
		//引数と違うタイプだった場合continue
		if (obj.first != type) {
			continue;
		}
		//指定されたタイプだった場合
		//そのオブジェクトのリストからモデルポインタを取得し
		//specificListに代入する
		for (auto& objSecond : obj.second) {
			specificList.push_back(objSecond->GetModelPointer());
		}
	}

	//リストを返す
	return specificList;
}

//キャラクター生成機
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

//敵生成機
void ObjectManager::EnemyGenerator(ObjectType objType)
{
	switch (objType) {
	case ObjectType::enemy:
		objects_[objType].push_front(std::make_shared<tempEnemy>(playerHandle_));
		break;
	}
}

//置物生成機
void ObjectManager::OrnamentGenerator(ObjectType objType)
{
	switch (objType) {
	case ObjectType::field:
		objects_[objType].push_front(std::make_shared<OrnamentBase>(fieldHandle_));
		break;
	}
}

//運べる置物生成機
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
