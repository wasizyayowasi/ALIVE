#include "ObjectManager.h"

#include "../object/Player.h"
#include "../object/EnemyBase.h"
#include "../object/ThrowEnemy.h"
#include "../object/ObjectBase.h"
#include "../object/DeadPerson.h"
#include "../object/OrnamentBase.h"

#include "../gimmick/Elevator.h"
#include "../gimmick/CrankScaffold.h"
#include "../gimmick/TransparentObject.h"
#include "../gimmick/PenetrationScaffld.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

#include <random>

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::MainStageObjectGenerator()
{

	auto& loadData = ExternalFile::GetInstance();

	for (auto& objInfo : loadData.GetLoadMainStageObjectInfo()) {
		if (objInfo.first == "BigBuildingA") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BigBuildingA,Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "SignBoard") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::SignBoardType1, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "BuildingC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BuildingCType1, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "Building2A") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BuildingAType2, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "StoreC") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::StoreC, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "Train") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Train, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "Station") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Station, Material::Stone, objSecond);
			}
		}
		else if (objInfo.first == "FenceType1") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Fence, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "Street") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Street, Material::Stone, objSecond);
			}
		}
		else if (objInfo.first == "TStreet") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::TStreet, Material::Stone, objSecond);
			}
		}
		else if (objInfo.first == "Tile") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Tile, Material::Stone, objSecond);
			}
		}
		else if (objInfo.first == "TileType2") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::TileType2, Material::Stone, objSecond);
			}
		}
		else if (objInfo.first == "BlueContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::BlueContainer, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "RedContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::RedContainer, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "YellowContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::YellowContainer, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "OrangeContainer") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::OrangeContainer, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "TunnelWall") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::TunnelWall, Material::Stone, objSecond);
			}
		}
		else if (objInfo.first == "TunnelEntrance") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::TunnelEntrance, Material::Stone, objSecond);
			}
		}
	}

	for (auto& gimmick : loadData.GetGimmickInfo()) {
		if (gimmick.first == "Trans") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::Trans, Material::Iron, objSecond);
			}
		}
		else if (gimmick.first == "CrankScaffold") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::CrankScaffold, Material::Iron, objSecond);
			}
		}
		else if (gimmick.first == "Elevator") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::Elevator, Material::Iron, objSecond);
			}
		}
		else if (gimmick.first == "PenetrationScaffld") {
			for (auto& objSecond : gimmick.second) {
				SortingObject(ObjectBaseType::GimmickBase, ObjectType::PenetrationScaffld, Material::Iron, objSecond);
			}
		}
	}
}

void ObjectManager::OpeningStageObjectGenerator()
{
	//短縮化
	auto& loadData = ExternalFile::GetInstance();

	for (auto& objInfo : loadData.GetLoadOpeningStageObjectInfo()) {
		if (objInfo.first == "Bed") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Bed, Material::Wood, objSecond);
			}
		}
		else if (objInfo.first == "RoomWall") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::RoomWall, Material::Stone, objSecond);
			}
		}
		else if (objInfo.first == "WoodFloor") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::WoodFloor, Material::Wood, objSecond);
			}
		}
		else if (objInfo.first == "Window") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Window, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "Clock") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Clock, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "Chair") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Chair, Material::Wood, objSecond);
			}
		}
		else if (objInfo.first == "Desk") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Desk, Material::Wood, objSecond);
			}
		}
		else if (objInfo.first == "WoodenBox") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::WoodenBox, Material::Wood, objSecond);
			}
		}
		else if (objInfo.first == "SignBoardType2") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::SignBoardType2, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "TV") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::TV, Material::Iron, objSecond);
			}
		}
		else if (objInfo.first == "Book") {
			for (auto& objSecond : objInfo.second) {
				SortingObject(ObjectBaseType::OrnamentBase, ObjectType::Book, Material::Other, objSecond);
			}
		}
	}
}

void ObjectManager::DeadPersonGenerator(int handle, LoadObjectInfo objInfo, int animNo)
{
	//死体を一つ生成する
	objects_[ObjectType::DeadPerson].push_back(std::make_shared<DeadPerson>(handle,Material::Other, objInfo, animNo));

	//死体が4個未満だったらリターン
	if(objects_[ObjectType::DeadPerson].size() < 4) return;

	//死体のリストの一番先頭(古い)死体を削除する
	objects_[ObjectType::DeadPerson].remove(objects_[ObjectType::DeadPerson].front());

}

//更新
void ObjectManager::Update(Player& player,std::shared_ptr<ShotManager> shotManager)
{
	//objects_の各要素のisEnableを取得し、無効になっていれば該当コンテナの削除
	for (auto& list : objects_) {
		list.second.remove_if([](std::shared_ptr<ObjectBase> obj) {return !obj->IsEnabled(); });
	}

	float distanceSize = 0.0f;
	VECTOR playerPos = player.GetStatus().pos;

	//死体とその他のオブジェクトの衝突判定を行う
	for (auto& list : objects_) {
		for (auto& obj : list.second) {
			if (list.first == ObjectType::DeadPerson) {
				continue;
			}
			for (auto& deadperson : objects_[ObjectType::DeadPerson]) {
				distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
				if (distanceSize < 4000.0f) {
					obj->UpdateForCorpse(deadperson);
				}
			}
		}
	}

	//enemyのShot
	for (auto& obj : objects_[ObjectType::Enemy]) {
		if (std::dynamic_pointer_cast<ThrowEnemy>(obj) != nullptr) {
			std::dynamic_pointer_cast<ThrowEnemy>(obj)->Shot(shotManager, player.GetStatus().pos, player.GetStatus().height);
		}
	}

	
	//更新
	for (auto& list : objects_) {
		for (auto& obj : list.second) {
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
			if (distanceSize < 4000) {
				obj->Update(player);
			}
		}
	}
	
	for (auto& enemy : ExternalFile::GetInstance().GetEnemyInfo(playerPos)) {
		if (!usedEnemyList_[enemy.name]) {
			EnemyGenerator(player.GetDeathCount(), enemy);
		}
	}
}

//描画
void ObjectManager::Draw(VECTOR PlayerPos)
{
	float distance = 0.0f;

	for (auto& objs : objects_) {
		for (auto& obj : objs.second) {
			//オブジェクトからプレイヤーまでの距離をとりサイズ変換する
			distance = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), PlayerPos);
			//プレイヤーから距離が8000.0f未満だったら描画する
			if (distance < 8000.0f) {
				obj->Draw();
			}
		}
	}

	int size = static_cast<int>(objects_[ObjectType::Enemy].size());
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

void ObjectManager::SortingObject(ObjectBaseType baseType, ObjectType objType, Material materialType, LoadObjectInfo objInfo)
{
	//objectBaseTypeを元にインスタンス化するクラスを決める
	switch (baseType) {
		//置物を生成
	case ObjectBaseType::OrnamentBase:
		OrnamentGenerator(objType, materialType, objInfo);
		break;
		//ギミックを生成
	case ObjectBaseType::GimmickBase:
		GimmickObjectGenerator(objType, materialType, objInfo);
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

	for (auto& obj : objects_) {
		for (auto& objSecond : obj.second) {
			if (objSecond->AddCollModel() != nullptr) {
				checkCollList_.push_back(objSecond->AddCollModel());
			}
		}
	}
}

//一定範囲内でランダムに召喚ポジションを取得する
void ObjectManager::RandomPositionGenerator(LoadObjectInfo& info, VECTOR loadObjPos)
{

	float distance = 500.0f;

	std::random_device random;
	std::mt19937 value(random());

	std::uniform_int_distribution<> randomPosX(loadObjPos.x - distance, loadObjPos.x);
	std::uniform_int_distribution<> randomPosZ(loadObjPos.z - distance, loadObjPos.z + distance);

	info.pos.x = static_cast<float>(randomPosX(value));
	info.pos.z = static_cast<float>(randomPosZ(value));

}

//エネミーを召喚させるポジションを角度によって取得する
void ObjectManager::CircumferencePosition(float angle, VECTOR& infoPos, VECTOR playerPos)
{
	VECTOR pos = {};
	float radian = MathUtil::DegreeToRadian(angle);

	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	pos = VScale(pos,200.0f);

	pos.y = infoPos.y;

	pos = VAdd(playerPos, pos);

	pos.y = infoPos.y;

	infoPos = pos;
}

//敵生成機
void ObjectManager::EnemyGenerator(int deathCount, LoadObjectInfo info)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	//文字列のサイズを取得する
	int size = static_cast<int>(info.name.size());

	//「.」以降の文字列によって
	//エネミーの召喚パターンを変更する
	if (size > 0) {
		std::string str = StrUtil::GetStringAfterSign(info.name, ".");
		str = StrUtil::GetStringBeforeSign(str, "-");

		//文字列が「ALL」だったら
		if (str == "ALL") {
			float angle = 0.0f;
			for (int i = 0; i < deathCount; i++) {
				//一定範囲の中でランダムにスポーンさせる
				//RandomPositionGenerator(info, loadInfo.pos);
				
				//プレイヤーを中心に円周上でスポーンさせる
				CircumferencePosition(angle, info.pos, info.pos);

				//インスタンス化
				objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(ObjectType::Enemy),Material::Other, info));
				angle -= 15.0f;

				usedEnemyList_[info.name] = true;
			}
		}
		else {
			//最後の文字列をint型の数値に変換する
			int num = atoi(str.c_str());

			//文字列の最後の数よりもdeathCountが多ければ
			//エネミーを召喚する
			if (num <= deathCount) {
				objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(ObjectType::Enemy), Material::Other, info));
				usedEnemyList_[info.name] = true;
			}
		}
	}
}

//置物生成機
void ObjectManager::OrnamentGenerator(ObjectType objType,Material materialType, LoadObjectInfo objInfo)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	objects_[objType].push_front(std::make_shared<OrnamentBase>(model.GetModelHandle(objType), materialType, objInfo));
}

void ObjectManager::GimmickObjectGenerator(ObjectType objType, Material materialType, LoadObjectInfo objInfo)
{
	//短縮化
	auto& model = ModelManager::GetInstance();

	switch (objType) {
	case ObjectType::Trans:
		objects_[objType].push_front(std::make_shared<TransparentObject>(model.GetModelHandle(objType),materialType, objInfo));
		break;
	case ObjectType::Elevator:
		objects_[objType].push_front(std::make_shared<Elevator>(model.GetModelHandle(objType),materialType, objInfo));
		break;
	case ObjectType::CrankScaffold:
		objects_[objType].push_front(std::make_shared<CrankScaffold>(model.GetModelHandle(objType),materialType, objInfo));
		break;
	case ObjectType::PenetrationScaffld:
		objects_[objType].push_front(std::make_shared<PenetrationScaffld>(model.GetModelHandle(ObjectType::Train), materialType, objInfo));
		break;
	}
}
