#include "ObjectManager.h"

#include "../object/ObjectBase.h"
#include "../object/EnemyBase.h"
#include "../object/DeadPerson.h"
#include "../object/tempEnemy.h"
#include "../object/OrnamentBase.h"

#include "../gimmick/TransparentObject.h"
#include "../gimmick/Steelyard.h"
#include "../gimmick/Elevator.h"

#include "InputState.h"
#include "ExternalFile.h"

namespace {
	const char* const player_Filename = "data/player/player16.mv1";
	//仮モデルのファイルパス
	const char* const temp_fieldpath = "data/model/tempFiled4.mv1";
	const char* const temp_stairs = "data/model/stairs.mv1";
	const char* const bigPillar_filename = "data/level0_model/bigPillar.mv1";
	const char* const switch_filename = "data/model/switch.mv1";
	const char* const steelyard_filename = "data/model/steelyard.mv1";
	const char* const transparent_obj_filename = "data/model/trans.mv1";
	const char* const elevator_obj_filename = "data/model/elevator.mv1";

	//実際に使う予定のモデルパス
	//でかいビル
	const char* const big_buildingA_filepath = "data/model/city/BigBuildingA.mv1";
	const char* const big_buildingB_filepath = "data/model/city/BigBuildingB.mv1";
	const char* const big_buildingC_filepath = "data/model/city/BigBuildingC.mv1";
	const char* const big_buildingD_filepath = "data/model/city/BigBuildingD.mv1";
	//Aみたいな形のビル
	const char* const buildingA_filepath = "data/model/city/BuildingA.mv1";
	const char* const buildingB_filepath = "data/model/city/BuildingB.mv1";
	const char* const buildingC_filepath = "data/model/city/BuildingC.mv1";
	//長方形のビル
	const char* const buildingA_type2_filepath = "data/model/city/Building2A.mv1";
	const char* const buildingB_type2_filepath = "data/model/city/Building2B.mv1";
	const char* const buildingC_type2_filepath = "data/model/city/Building2C.mv1";
	//海外で見るような飲食店
	const char* const storeA_filepath = "data/model/city/StoreA.mv1";
	const char* const storeB_filepath = "data/model/city/StoreB.mv1";
	const char* const storeC_filepath = "data/model/city/StoreC.mv1";
	//道
	const char* const street_filepath = "data/model/city/Street.mv1";
	const char* const T_street_filepath = "data/model/city/TStreet.mv1";
	const char* const Tile_filepath = "data/model/city/Tile.mv1";
	const char* const scaffold_filepath = "data/model/city/others/Scaffold.mv1";
	const char* const slopeScaffold_filepath = "data/model/city/others/SlopeScaffold.mv1";
	const char* const fence_filepath = "data/model/city/others/Fence.mv1";
	//建物
	const char* const BlueContainer_filepath = "data/model/city/container/mv1/BlueContainer.mv1";
	const char* const RedContainer_filepath = "data/model/city/container/mv1/RedContainer.mv1";
	const char* const YellowContainer_filepath = "data/model/city/container/mv1/YellowContainer.mv1";
	const char* const OrangeContainer_filepath = "data/model/city/container/mv1/OrangeContainer.mv1";
	const char* const PurpleContainer_filepath = "data/model/city/container/mv1/PurpleContainer.mv1";
	const char* const GreenContainer_filepath = "data/model/city/container/mv1/GreenContainer.mv1";
}

ObjectManager::ObjectManager()
{
	playerHandle_ = MV1LoadModel(player_Filename);
	fieldHandle_ = MV1LoadModel(temp_fieldpath);
	switchHandle_ = MV1LoadModel(switch_filename);
	steelyardHandle_ = MV1LoadModel(steelyard_filename);
	transObjHandle_ = MV1LoadModel(transparent_obj_filename);
	elevatorHandle_ = MV1LoadModel(elevator_obj_filename);

	modelHandle_[ObjectType::BigBuildingA] = MV1LoadModel(big_buildingA_filepath);
	modelHandle_[ObjectType::BigBuildingB] = MV1LoadModel(big_buildingB_filepath);
	modelHandle_[ObjectType::BigBuildingC] = MV1LoadModel(big_buildingC_filepath);
	modelHandle_[ObjectType::BigBuildingD] = MV1LoadModel(big_buildingD_filepath);

	modelHandle_[ObjectType::BuildingAType1] = MV1LoadModel(buildingA_filepath);
	modelHandle_[ObjectType::BuildingBType1] = MV1LoadModel(buildingB_filepath);
	modelHandle_[ObjectType::BuildingCType1] = MV1LoadModel(buildingC_filepath);

	modelHandle_[ObjectType::BuildingAType2] = MV1LoadModel(buildingA_type2_filepath);
	modelHandle_[ObjectType::BuildingBType2] = MV1LoadModel(buildingB_type2_filepath);
	modelHandle_[ObjectType::BuildingCType2] = MV1LoadModel(buildingC_type2_filepath);

	modelHandle_[ObjectType::StoreA] = MV1LoadModel(storeA_filepath);
	modelHandle_[ObjectType::StoreB] = MV1LoadModel(storeB_filepath);
	modelHandle_[ObjectType::StoreC] = MV1LoadModel(storeC_filepath);

	modelHandle_[ObjectType::Street] = MV1LoadModel(street_filepath);
	modelHandle_[ObjectType::TStreet] = MV1LoadModel(T_street_filepath);
	modelHandle_[ObjectType::Tile] = MV1LoadModel(Tile_filepath);
	modelHandle_[ObjectType::Scaffold] = MV1LoadModel(scaffold_filepath);
	modelHandle_[ObjectType::SlopeScaffold ] = MV1LoadModel(slopeScaffold_filepath);
	modelHandle_[ObjectType::Fence] = MV1LoadModel(fence_filepath);

	modelHandle_[ObjectType::BlueContainer] = MV1LoadModel(BlueContainer_filepath);
	modelHandle_[ObjectType::RedContainer] = MV1LoadModel(RedContainer_filepath);
	modelHandle_[ObjectType::YellowContainer] = MV1LoadModel(YellowContainer_filepath);
	modelHandle_[ObjectType::OrangeContainer] = MV1LoadModel(OrangeContainer_filepath);
	modelHandle_[ObjectType::PurpleContainer] = MV1LoadModel(PurpleContainer_filepath);
	modelHandle_[ObjectType::GreenContainer] = MV1LoadModel(GreenContainer_filepath);

}

ObjectManager::~ObjectManager()
{
	MV1DeleteModel(playerHandle_);
	MV1DeleteModel(fieldHandle_);
	MV1DeleteModel(switchHandle_);
	MV1DeleteModel(steelyardHandle_);
	MV1DeleteModel(transObjHandle_);
	MV1DeleteModel(elevatorHandle_);

	for (auto& type : modelHandle_) {
		MV1DeleteModel(type.second);
	}

}

void ObjectManager::ObjectGenerator()
{

	auto& loadData = ExternalFile::GetInstance();

	for (auto& objInfo : loadData.GetLoadObjectInfo()) {
		//フィールドを作成
		if (objInfo.first == "field") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::field, objSecond);
			}
		}
		//箱を作成
		else if (objInfo.first == "box") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::carryBase, ObjectType::carry, objSecond);
			}
		}
		//敵を作成
		else if (objInfo.first == "Enemy") {
			for (auto& objSecond : objInfo.second) {
 				SortingObject(ObjectBaseType::enemyBase, ObjectType::enemy, objSecond);
			}
		}
		else if (objInfo.first == "BigBuildingA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingA, objSecond);
			}
		}
		else if (objInfo.first == "BigBuildingB") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingB, objSecond);
			}
		}
		else if (objInfo.first == "BigBuildingC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingC, objSecond);
			}
		}
		else if (objInfo.first == "BigBuildingD") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BigBuildingD, objSecond);
			}
		}
		else if (objInfo.first == "BuildingA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingAType1, objSecond);
			}
		}
		else if (objInfo.first == "BuildingB") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingBType1, objSecond);
			}
		}
		else if (objInfo.first == "BuildingC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingCType1, objSecond);
			}
		}
		else if (objInfo.first == "Building2A") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingAType2, objSecond);
			}
		}
		else if (objInfo.first == "Building2B") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingBType2, objSecond);
			}
		}
		else if (objInfo.first == "Building2C") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BuildingCType2, objSecond);
			}
		}
		else if (objInfo.first == "StoreA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::StoreA, objSecond);
			}
		}
		else if (objInfo.first == "StoreB") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::StoreB, objSecond);
			}
		}
		else if (objInfo.first == "StoreC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::StoreC, objSecond);
			}
		}
		else if (objInfo.first == "Street") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::Street, objSecond);
			}
		}
		else if (objInfo.first == "TStreet") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::TStreet, objSecond);
			}
		}
		else if (objInfo.first == "Tile") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::Tile, objSecond);
			}
		}
		else if (objInfo.first == "Scaffold") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::Scaffold, objSecond);
			}
		}
		else if (objInfo.first == "SlopeScaffold") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::SlopeScaffold, objSecond);
			}
		}
		else if (objInfo.first == "Fence") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::Fence, objSecond);
			}
		}
		else if (objInfo.first == "BlueContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::BlueContainer, objSecond);
			}
		}
		else if (objInfo.first == "RedContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::RedContainer, objSecond);
			}
		}
		else if (objInfo.first == "YellowContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::YellowContainer, objSecond);
			}
		}
		else if (objInfo.first == "OrangeContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::OrangeContainer, objSecond);
			}
		}
		else if (objInfo.first == "PurpleContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::PurpleContainer, objSecond);
			}
		}
		else if (objInfo.first == "GreenContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::ornamentBase, ObjectType::GreenContainer, objSecond);
			}
		}
	}

	for (auto& gimmick : loadData.GetGimmickInfo()) {
		//ギミックスイッチを作成
		if (gimmick.first == "Trans") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::gimmickBase, ObjectType::trans, objSecond);
			}
		}
		//ギミック天秤を作成
		else if (gimmick.first == "Steelyard") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard, objSecond);
			}
		}
		else if (gimmick.first == "Elevator") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::gimmickBase, ObjectType::elevator, objSecond);
			}
		}
	}

}

void ObjectManager::DeadPersonGenerator(int handle, LoadObjectInfo objInfo, int animNo)
{
	objects_[ObjectType::deadPerson].push_back(std::make_shared<DeadPerson>(handle, objInfo, animNo));

	if(objects_[ObjectType::deadPerson].size() < 4) return;

	objects_[ObjectType::deadPerson].remove(objects_[ObjectType::deadPerson].front());

}

//更新
void ObjectManager::Update(Player& player, const InputState& input)
{
	//objects_の各要素のisEnableを取得し、無効になっていれば該当コンテナの削除
	std::erase_if(objects_, [](const auto& obj) {return !obj.second.front()->IsEnabled(); });

	//死体のポインターを収集する
	std::list<std::shared_ptr<ObjectBase>> deadPerson = {};
	for (auto obj : objects_) {
		for (auto objSecond : obj.second) {
			if (obj.first == ObjectType::deadPerson) {
				deadPerson.push_back(objSecond);
			}
		}
	}

	//死体とその他のオブジェクトの衝突判定を行う
	for (auto& obj : objects_) {
		for (auto& objSecond : obj.second) {
			if (obj.first != ObjectType::deadPerson) {
				for (auto& dead : deadPerson) {
					objSecond->HitColl(dead);
				}
			}
		}
	}

	//更新
	for (auto obj : objects_) {
		for (auto objSecond : obj.second) {
			objSecond->Update(player,input);
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

	for (auto& obj : objects_) {
		for (auto& model : obj.second) {
			if (model->IsCollCheck()) {
				checkCollList_.push_back(model->GetModelPointer());
			}
		}
	}

	return checkCollList_;
}

void ObjectManager::SortingObject(ObjectBaseType baseType, ObjectType objType, LoadObjectInfo objInfo)
{
	//objectBaseTypeを元にインスタンス化するクラスを決める
	switch (baseType) {

		//自我を持ったenemy以外のキャラクターを生成
	case ObjectBaseType::characterBase:
		CharacterGenerator(objType, objInfo);
		break;

		//enemyを生成
	case ObjectBaseType::enemyBase:
		EnemyGenerator(objType, objInfo);
		break;

		//置物を生成
	case ObjectBaseType::ornamentBase:
		OrnamentGenerator(objType, objInfo);
		break;

		//運べる置物生成
	case ObjectBaseType::carryBase:
		CarryObjectGenerator(objType, objInfo);
		break;

		//ギミックを生成
	case ObjectBaseType::gimmickBase:
		GimmickObjectGenerator(objType, objInfo);
		break;
	}
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

std::list<std::shared_ptr<ObjectBase>> ObjectManager::GetSpecificObject(ObjectType type)
{

	std::list<std::shared_ptr<ObjectBase>> obj;

	if (objects_.count(type) > 0) {
		obj = objects_[type];
	}

	return obj;
}

void ObjectManager::AddCheckCollModel()
{

	checkCollList_.clear();

	for (auto obj : objects_) {
		for (auto objSecond : obj.second) {
			if (objSecond->AddCollModel() != nullptr) {
				checkCollList_.push_back(objSecond->AddCollModel());
			}
		}
	}
}

//キャラクター生成機
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

//敵生成機
void ObjectManager::EnemyGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType) {
	case ObjectType::enemy:
		objects_[objType].push_front(std::make_shared<TempEnemy>(playerHandle_, objInfo));
		break;
	}
}

//置物生成機
void ObjectManager::OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo)
{

	objects_[objType].push_front(std::make_shared<OrnamentBase>(modelHandle_[objType], objInfo));

}

//運べる置物生成機
void ObjectManager::CarryObjectGenerator(ObjectType objType, LoadObjectInfo objInfo)
{
	switch (objType) {
	case ObjectType::carry:
		//objects_[objType].push_front(std::make_shared <CarryObjectBase>(boxHandle_, objInfo));
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
		break;
	case ObjectType::elevator:
		objects_[objType].push_front(std::make_shared<Elevator>(elevatorHandle_, objInfo));
		break;
	}
}
